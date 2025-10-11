//
// Created by Oliver on 09/10/2025.
//
/*
 * 1. Hvor mange states kan fuglen ha?
 *      -> Død
 *      -> Spillende
 *
 * 2. Hvilke verdier skal den ha?
 *      -> y-velocity
 *      -> y-position
 *      -> size ?
 *      -> bird_state (dead, alive)
 *
 * 3. Hvilke metoder skal den ha?
 *      -> flap()
 *          --> endrer y-velocity
 *          --> kanskje ha en flapcooldown?
 *      -> kill()
 *          --> bird_state = dead
 *      -> update(dt)
            --> if bird_state = alive:
 *                  velocity = gravity * dt;
 *                  position = velocity * dt;
 */

#pragma once
#include "constants.hpp"

enum class BirdState {ALIVE, DEAD};

struct Bird {
    float vel_y = 0.0f;
    float pos_y = 0.0f;
    float rot_z = 0.0f;
    BirdState bird_state = BirdState::ALIVE;

    void flap() {
        if (bird_state == BirdState::ALIVE) {
            vel_y = birdcfg::FLAP_VEL;
        }
    }

    void kill() {
        bird_state = BirdState::DEAD;
    }

    // handles rotation with max/min rotaion
    void setRotation(float velocity) {
        rot_z = velocity*0.01f*threepp::math::PI;
        if (rot_z < -2/threepp::math::PI) {
            rot_z = -2/threepp::math::PI;
        } else if (rot_z > (1/threepp::math::PI)) {
            rot_z = 1/threepp::math::PI;
        }
    }

    void update(float dt) {
        if (bird_state == BirdState::ALIVE) {
        vel_y += birdcfg::GRAVITY * dt;
        pos_y += vel_y * dt;
        setRotation(vel_y);
        }
    }
};

class BirdInput : public threepp::KeyListener {
public:
    explicit BirdInput(Bird& b): bird(b) {}
    void onKeyPressed(threepp::KeyEvent e) override {
        if (e.key == threepp::Key::SPACE) {
            bird.flap();
        }
    }
private:
    Bird& bird; // koden funket ikke fordi denne lagde en kopi istedenfor referanse
};