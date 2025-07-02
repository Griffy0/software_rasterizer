#pragma once
#include "math_helpers.hpp"

const size_t WIDTH = 640;
const size_t HEIGHT = 640;
const float FRUSTRUM_DEPTH = 700;
const float FRUSTRUM_NEAR_DEPTH = 0.1f;
const float FRUSTRUM_WIDTH = 2.0f;
const float FRUSTRUM_HEIGHT = 2.0f;
const size_t TEXTURE_WIDTH = 32;
const size_t TEXTURE_HEIGHT = 48;
const float fov_x = deg_to_rad(90.0);
const float zoom_x = 1/(tan(fov_x/2));
const float fov_y = fov_x * HEIGHT / WIDTH;
const float zoom_y = 1/(tan(fov_y/2));


