#pragma once
#include "graphic_structs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>

bool load_bmp_24(const char* filename, Texture& out_texture);