#pragma once
#include "graphic_structs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <map>
#include <filesystem>


bool load_bmp_24(const char* filename, Texture* out_texture);
map<string, Texture*> load_textures();