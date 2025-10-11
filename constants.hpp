//
// Created by Oliver on 09/10/2025.
//

#pragma once

namespace birdcfg {
    constexpr float GRAVITY = -20.0f;
    constexpr float FLAP_VEL = 10.0f;
}

namespace pipecfg {
    constexpr int NUM_PIPES         = 6;
    constexpr float PIPE_SPEED      =-10.0f;   // negativ for å bevege seg mot venstre
    constexpr float PIPE_RADIUS     = 1.0f;
    constexpr float PIPE_HEIGHT     = 20.0f;
    constexpr float SPACING         = 20.0f;    // avstand mellom pipe-par
    constexpr float RESPAWN_X       = 30.0f;    // hvor pipes respawner til høyre
    constexpr float DESPAWN_X       = RESPAWN_X - NUM_PIPES*SPACING;   // når pipes er forbi kamera til venstre
    constexpr float GAP_SIZE        = 4.0f;     // vertikal gap mellom topp og bunn
    constexpr float RAND_MIN_Y      =-7.0f;   // random range for senter-Y
    constexpr float RAND_MAX_Y      = 8.0f;
}