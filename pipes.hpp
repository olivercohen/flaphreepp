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

inline std::random_device rd;
inline std::mt19937 gen(rd());
inline std::uniform_int_distribution<> distrib(pipecfg::RAND_MIN_Y, pipecfg::RAND_MAX_Y);

enum class PipesState{ONSCREEN, OFFSCREEN, FREEZE};

struct PipePair {
    float pos_y = 0.0f;
    float pos_x = 0.0f;
    float speed = pipecfg::PIPE_SPEED;
    PipesState pipe_state = PipesState::ONSCREEN;

    void freeze() {
        speed = 0.0f;
        pipe_state = PipesState::FREEZE;
    }

    void spawn(const float x) {
        pos_x = x;
        pos_y = static_cast<float>(distrib(gen));
        speed = pipecfg::PIPE_SPEED;
    }

    bool isOffscreen() {
        return (pos_x < pipecfg::DESPAWN_X);
    }

    void update(float dt) {
        if (pipe_state == PipesState::FREEZE) {return;};
        pos_x += speed * dt;
    }
};

inline std::shared_ptr<threepp::Group> createPipeGroup(
    auto &pipe_geom,
    auto &pipe_mat) {

    auto group = threepp::Group::create();

    auto top = threepp::Mesh::create(pipe_geom, pipe_mat);
    auto bot = threepp::Mesh::create(pipe_geom, pipe_mat);

    top->castShadow = true;
    top->receiveShadow = true;
    bot->castShadow = true;
    bot->receiveShadow = true;

    top->position.set(0, (pipecfg::PIPE_HEIGHT + pipecfg::GAP_SIZE)/2, 0);
    bot->position.set(0, -(pipecfg::PIPE_HEIGHT + pipecfg::GAP_SIZE)/2, 0);

    group->add(top);
    group->add(bot);

    return group;
}