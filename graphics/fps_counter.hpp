#pragma once
#include <SDL3/SDL.h>

class Performance{
    public:
        int frameCount;
        Uint64 startTime;
        Uint64 frequency;
        Uint64 currentTime;
        Uint64 elapsedTime;
        Performance();
};

void update_fps(Performance& frame_counter);