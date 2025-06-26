#include "math_helpers.h"
#include <random>
#include <cmath>
#include <algorithm>

double deg_to_rad(double degree){
    double pi = 3.14159265359;
    return (degree * (pi / 180));
};

int randint(int min, int max){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); // distribution in range [1, 6]
    return dist6(rng);
};

uint8_t rescale(float num, float current_max, float max){
    return static_cast<uint8_t> (max/current_max)*num;
};

int rescale_int(float num, float current_max, float max){
    //return static_cast<int> (round((max/current_max)*num));
    return static_cast<int> (std::min(floor(num*max), max-1));//(ceil(2.0f*((max+1)/(current_max+1))*num)-1);
};//(round((num*max)/current_max));//(std::min(