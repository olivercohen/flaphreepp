#include <iostream>
#include "threepp/threepp.hpp"
#include "bird.hpp"
#include "pipes.hpp"
#include "collisions.hpp"
#include <vector>
#include <memory>

int main() {
    // THREEPP CONFIG
    threepp::Canvas canvas("Flappy_Demo 0.1");
    threepp::GLRenderer renderer{canvas.size()};
    renderer.setClearColor(threepp::Color::skyblue);
    renderer.autoClear = false;


    auto scene = threepp::Scene::create();
    auto camera = threepp::PerspectiveCamera::create(30, canvas.aspect(), 0.1f, 100.f);
    camera->position.set(0, 0, 45.f);
    threepp::OrbitControls controls{*camera, canvas};
    scene->fog = threepp::Fog(threepp::Color::skyblue, 43.f, 55.f);

    // HUD / POINTS DISPLAY + LOGIC
    threepp::HUD hud(canvas.size());
    threepp::FontLoader fontLoader;
    const auto font = fontLoader.defaultFont();
    threepp::TextGeometry::Options opts(font, 25, 5);

    int points = 0;
    auto points_label = threepp::Text2D(opts, std::to_string(points));
    points_label.setColor(threepp::Color::black);

    hud.add(points_label, threepp::HUD::Options()
            .setNormalizedPosition({0.5f, 0.95f}));

    //LYS + SHADOWS
    auto hemi_light = threepp::HemisphereLight::create(threepp::Color::white, threepp::Color::darkslategray, 0.5f);
    hemi_light->intensity = 0.7f;
    scene->add(hemi_light);

    auto dir_light = threepp::DirectionalLight::create(threepp::Color::lightyellow, 0.9f);
    dir_light->position.set(10, 12, 8);
    dir_light->castShadow = true;
    // dir_light->shadow->mapSize.set(1028, 1028);

    // endrer på verdier i DirectionalLightShadow threepp biblioteket (tror jeg..?)
    if (auto *ortho = dynamic_cast<threepp::OrthographicCamera *>(dir_light->shadow->camera.get())) {
        ortho->left = -40.f;
        ortho->right = 40.f;
        ortho->top = 40.f;
        ortho->bottom = -40.f;
    }

    scene->add(dir_light);
    renderer.shadowMap().enabled = true;

    canvas.onWindowResize([&](threepp::WindowSize size) {
        hud.setSize(size);
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    // BIRD LOGIC / DRAW BIRD
    Bird bird;

    auto bird_body_mat = threepp::MeshStandardMaterial::create({{"color", threepp::Color::purple}});
    auto bird_body = threepp::Mesh::create(
        threepp::BoxGeometry::create(birdcfg::WIDTH, birdcfg::HEIGHT, birdcfg::DEPTH), bird_body_mat);

    bird_body->castShadow = true;
    bird_body->receiveShadow = false;
    bird_body->position.set(0, bird.pos_y, 0);
    bird_body->rotation.set(0, 0, bird.rot_z);

    scene->add(bird_body);

    // PIPE LOGIC / DRAW PIPE
    auto pipe_material = threepp::MeshStandardMaterial::create({{"color", threepp::Color::lightgreen}});
    auto pipe_geometry = threepp::CylinderGeometry::create(pipecfg::PIPE_RADIUS, pipecfg::PIPE_RADIUS,
                                                           pipecfg::PIPE_HEIGHT);

    pipe_material->roughness = 0.4f;
    pipe_material->metalness = 0.3f;

    std::vector<PipePair> pipes; // en pipe per pair
    std::vector<std::shared_ptr<threepp::Group> > pipeGroups;

    // disse er ikke egt nødvendig, men gjør det mer ryddig
    pipes.reserve(pipecfg::NUM_PIPES);
    pipeGroups.reserve(pipecfg::NUM_PIPES);

    for (int n = 1; n < pipecfg::NUM_PIPES + 1; ++n) {
        PipePair pipe;
        float x = pipecfg::RESPAWN_X + static_cast<float>(n) * pipecfg::SPACING;
        pipe.spawn(x);
        pipes.emplace_back(pipe);

        auto grp = createPipeGroup(pipe_geometry, pipe_material);
        grp->position.set(pipe.pos_x, pipe.pos_y, 0);
        scene->add(grp);
        pipeGroups.emplace_back(grp);
    }

    // Bird Input -> bird
    BirdInput input(bird);
    canvas.addKeyListener(input);

    // GROUND
    auto ground_material = threepp::MeshStandardMaterial::create({{"color", threepp::Color::darkgreen}});
    auto ground_geometry = threepp::BoxGeometry::create(groundcfg::WIDTH, groundcfg::HEIGHT, groundcfg::DEPTH);
    auto ground = threepp::Mesh::create(ground_geometry, ground_material);

    ground->position.set(0, groundcfg::POSITION_Y, 0);
    ground->castShadow = false;
    ground->receiveShadow = true;

    scene->add(ground);

    threepp::Clock clock;
    // canvas.animate() loopen som faktisk tegner
    canvas.animate([&]() {
        float dt = clock.getDelta();

        // BIRD UPDATES
        bird.update(dt);
        bird_body->position.y = bird.pos_y;
        bird_body->rotation.z = bird.rot_z;

        if (birdHasCollided(bird, pipes)) {
            bird.kill();
            for (int i = 0; i < pipecfg::NUM_PIPES; ++i) {
                auto &p = pipes[i];
                p.freeze();
            }
        } else {
            for (int i = 0; i < pipecfg::NUM_PIPES; ++i) {
                auto &p = pipes[i];
                auto &grp = pipeGroups[i];

                auto p_before = p;
                p.update(dt);

                if (p_before.pos_x > 0.0f && p.pos_x <= 0.0f) {
                    points += 1;
                    points_label.setText(std::to_string(points), opts);
                }

                grp->position.set(p.pos_x, p.pos_y, 0);

                if (p.isOffscreen()) {
                    p.spawn(pipecfg::RESPAWN_X);
                }
            }
        }

        renderer.clear();
        renderer.render(*scene, *camera); // tegn 3D-scenen
        hud.apply(renderer); // tegn HUD oppå (PROBLEM!! gjør at 3D-scenen ikke er synlig)
    });

    return 0;
}
