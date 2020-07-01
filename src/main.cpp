#define SDL_MAIN_HANDLED

#include "core.h"

int main() {
    bool burnMyGPU = false;

    if (burnMyGPU) {
        core->state.partitionsPerSide = 1;
        core->state.vsyncEnabled = false;
    } else {
        core->state.partitionsPerSide = 3;
        core->state.vsyncEnabled = true;
    }

    core->run();
}
