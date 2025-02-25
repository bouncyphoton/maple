#ifndef MAPLE_GAME_STATE_H
#define MAPLE_GAME_STATE_H

#include "types.h"
#include "camera.h"
#include "scene.h"

// Events from the platform abstraction to the application
enum PlatformEventsEnum : u32 {
    PLATFORM_EVENT_FRAME_RESIZED = 0x01
};

struct GameState {
    u32 frameWidth = 800;
    u32 frameHeight = 600;
    u32 partitionsPerSide = 16;
    bool vsyncEnabled = false;

    // Bitfield for platform events, see PlatformEventsEnum
    u32 platformEventFlags = 0;

    Camera camera = {};
    Scene scene = {};
};

#endif //MAPLE_GAME_STATE_H
