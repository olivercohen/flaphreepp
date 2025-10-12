//
// Created by Oliver on 12/10/2025.
//

#pragma once
#include "bird.hpp"
#include "pipes.hpp"

inline bool birdHitsGround(const Bird& bird) {
    if (bird.pos_y + birdcfg::HITBOX_RADIUS <= groundcfg::POSITION_Y + groundcfg::HEIGHT) {
        return true;
    }
    return false;
}

inline bool birdHitsPipe(const Bird& bird, auto pipes) {
    for (int i = 0; i < pipecfg::NUM_PIPES; ++i) {
        auto& p = pipes[i];

        if (std::fabs(p.pos_x) <= (birdcfg::WIDTH*0.5f + pipecfg::PIPE_RADIUS)) {
            if (std::fabs(bird.pos_y - p.pos_y) > pipecfg::GAP_SIZE*0.5f - birdcfg::HEIGHT*0.5f) {
                return true;
            }
        }
    }
    return false;
}

inline bool birdHasCollided(const Bird& bird, auto pipes) {
    if (birdHitsGround(bird) || birdHitsPipe(bird, pipes)) {
        return true;
    }
    return false;
}
