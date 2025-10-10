//
// Created by Oliver on 10/10/2025.
//
/*
 * 1. Hvor mange states skal pipesene ha?
 *      -> onscreen (når pipesene er på vei mot fuglen)
 *      -> offscreen (når de er gått av skjermen bak fuglen)
 *      -> frozen (når kollisjon oppstår med fuglen)
 *
 * 2. Hvilke verdier skal pipesene ha?
 *      -> posisjon y (max høyde, min høyde)
 *      -> pos_x
 *      -> fart_x
 *      -> kanskje en rotasjon når man kommer lengre inn i spillet (high score øker?)
 *      -> pipe_state
 *
 * 3. Hvilke metoder skal de ha?
 *      -> freeze() - setter pipe_state til FREEZE
 *      -> update(dt) - endrer på posisjon med hensyn på fart_x, sjekker pipes_state og kaller metoder om nødvendig
 *      -> spawn() - når pipen skal dannes, setter pipe til offscreen (pos_x) og setter en tilfeldig høyde pos_y
 *      -> notVisible() - setter pipe_state til OFFSCREEN
 *      ->
 */

#pragma once
#include <random>
#include "constants.hpp"

enum class PipesState{ONSCREEN, OFFSCREEN, FREEZE};

inline std::random_device rd;
inline std::mt19937 gen(rd());
inline std::uniform_int_distribution<> distrib(-10, 10);

struct PipePair {
    float pos_y = 0.0f;
    float pos_x = 0.0f;
    float speed = constants::PIPE_SPEED;
    PipesState pipe_state = PipesState::ONSCREEN;

    void freeze() {
        speed = 0.0f;
        pipe_state = PipesState::FREEZE;
    }

    void spawn() {
        pos_x = 30.0f;
        pos_y = static_cast<float>(distrib(gen));
        speed = constants::PIPE_SPEED;
    }

    void visCheck() {
        if (pos_x < -35) {
            pipe_state = PipesState::OFFSCREEN;
            spawn();
        }
    }
    void update(float dt) {
        pos_x += speed * dt;
    }
};