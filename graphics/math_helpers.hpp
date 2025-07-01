#pragma once
#include <random>
#include <cmath>
#include <algorithm>

double deg_to_rad(double degree);
int randint(int min, int max);
uint8_t rescale(float num, float current_max, float max);
int rescale_int(float num, float current_max, float max);