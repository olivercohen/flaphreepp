#include <iostream>
#include "threepp/threepp.hpp"
#include "bird.hpp"
#include "pipes.hpp"
#include <vector>
#include <memory>

int main() {
    // THREEPP CONFIG
    threepp::Canvas canvas("Flappy_Demo 0.1");
    threepp::GLRenderer renderer{canvas.size()};
    renderer.setClearColor(threepp::Color::blanchedalmond);

    auto scene = threepp::Scene().create();
    auto camera = threepp::PerspectiveCamera::create(30, canvas.aspect(), 0.1f, 100.f);
    camera->position.set(0, 0, 45.f);
    threepp::OrbitControls controls{*camera, canvas};

    //LYS
    auto hemi_light = threepp::HemisphereLight::create(threepp::Color::white, threepp::Color::darkslategray, 0.5f);
    hemi_light->intensity = 0.7f;
    scene->add(hemi_light);

    auto dir_light = threepp::DirectionalLight::create(threepp::Color::lightyellow, 0.8f);
    dir_light->position.set(10,12,8);
    dir_light->castShadow = true;

    scene->add(dir_light);

    // auto grid = threepp::GridHelper::create(20,20);
    // scene->add(grid);

    canvas.onWindowResize([&](threepp::WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    // BIRD LOGIC / DRAW BIRD
    Bird bird;

    auto bird_body_mat = threepp::MeshStandardMaterial::create({{"color",threepp::Color::purple}});
    auto bird_body = threepp::Mesh::create(threepp::BoxGeometry::create(1,1,1), bird_body_mat);
    bird_body->position.set(0, bird.pos_y, 0);
    bird_body->rotation.set(0,0,bird.rot_z);
    scene->add(bird_body);

    // PIPE LOGIC / DRAW PIPE

    auto pipe_material = threepp::MeshStandardMaterial::create({{"color", threepp::Color::lightgreen}});
    auto pipe_geometry = threepp::CylinderGeometry::create(pipecfg::PIPE_RADIUS,pipecfg::PIPE_RADIUS, pipecfg::PIPE_HEIGHT);

    std::vector<PipePair> pipes;    // en pipe per pair
    std::vector<std::shared_ptr<threepp::Group>> pipeGroups;

    // disse er ikke egt nødvendig, men gjør det mer ryddig
    pipes.reserve(pipecfg::NUM_PIPES);
    pipeGroups.reserve(pipecfg::NUM_PIPES);

    for (int n = 1; n < pipecfg::NUM_PIPES+1; ++n) {
        PipePair pipe;
        float x = pipecfg::RESPAWN_X + static_cast<float>(n) * pipecfg::SPACING;
        pipe.spawn(x);
        pipes.emplace_back(pipe);

        auto grp = createPipeGroup(pipe_geometry, pipe_material);
        grp->position.set(pipe.pos_x, pipe.pos_y, 0);
        scene->add(grp);
        pipeGroups.emplace_back(grp);
    }

    // Input -> bird
    BirdInput input(bird);
    canvas.addKeyListener(input);

    // Ground
    auto ground_material = threepp::MeshStandardMaterial::create({{"color", threepp::Color::darkgrey}});
    auto ground_geometry = threepp::BoxGeometry::create(40,3,4);
    auto ground = threepp::Mesh::create(ground_geometry, ground_material);
    ground->position.set(0,-11,0);

    scene->add(ground);

    threepp::Clock clock;
    // canvas.animate() loopen som faktisk tegner
    canvas.animate([&]() {
        double dt = clock.getDelta();

        bird.update(static_cast<float>(dt));
        bird_body->position.y = bird.pos_y;
        bird_body->rotation.z = bird.rot_z;

        for (int i = 0; i < pipecfg::NUM_PIPES; ++i) {
            auto& p = pipes[i];
            auto& grp = pipeGroups[i];

            p.update(dt);
            grp->position.set(p.pos_x, p.pos_y, 0);

            if (p.isOffscreen()) {
                p.spawn(pipecfg::RESPAWN_X);
            }
        }

        renderer.render(*scene, *camera); //
    });

    return 0;
}