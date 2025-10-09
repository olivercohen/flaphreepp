#include <iostream>
#include "threepp/threepp.hpp"
#include "bird.hpp"

int main() {
    // THREEPP CONFIG
    threepp::Canvas canvas("Flappy_Demo 0.1");
    threepp::GLRenderer renderer{canvas.size()};
    renderer.setClearColor(threepp::Color::lightblue);

    auto scene = threepp::Scene().create();
    auto camera = threepp::PerspectiveCamera::create(30, canvas.aspect(), 0.1f, 100.f);
    camera->position.set(0, 0, 45.f);
    threepp::OrbitControls controls{*camera, canvas};

    auto light = threepp::HemisphereLight::create();
    light->intensity = 0.7f;
    scene->add(light);

    // auto grid = threepp::GridHelper::create(20,20);
    // grid->rotation.x = 1;
    // scene->add(grid);

    canvas.onWindowResize([&](threepp::WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    // BIRD LOGIC / DRAW BIRD
    Bird bird;

    auto bird_body_mat = threepp::MeshStandardMaterial::create({{"color",threepp::Color::purple}});
    auto bird_body = threepp::Mesh::create(threepp::BoxGeometry::create(1,1,1,1), bird_body_mat);
    bird_body->position.set(0, bird.pos_y, 0);
    bird_body->rotation.set(0,0,bird.rot_z);
    scene->add(bird_body);

    // Input -> bird
    BirdInput input(bird);
    canvas.addKeyListener(input);

    threepp::Clock clock;
    // canvas.animate() loopen som faktisk tegner
    canvas.animate([&]() {
        double dt = clock.getDelta();

        bird.update(dt);

        bird_body->position.y = bird.pos_y;
        bird_body->rotation.z = bird.rot_z;

        renderer.render(*scene, *camera); //
    });

    return 0;
}