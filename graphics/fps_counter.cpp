#include <SDL3/SDL.h>
#include <iostream> //cout
#include <fstream> //endl
#include "fps_counter.hpp"

Performance::Performance(){
    startTime = SDL_GetPerformanceCounter();
    frameCount = 0;
    frequency = SDL_GetPerformanceFrequency();
};

void update_fps(Performance& frame_counter){
    frame_counter.frameCount++;
    Uint64 currentTime = SDL_GetPerformanceCounter();
    Uint64 elapsedTime = currentTime - frame_counter.startTime;
    if (elapsedTime >= frame_counter.frequency) {
        float fps = frame_counter.frameCount / (elapsedTime / (float)frame_counter.frequency); // FPS = frameCount / seconds
        std::cout << "FPS: " << fps << std::endl;
        frame_counter.frameCount = 0;
        frame_counter.startTime = currentTime;
    }
};
