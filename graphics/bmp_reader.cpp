#include "bmp_reader.h"
#include "settings.h"
#include "graphic_structs.h"#include <fstream>
#include <cstdint>
#include <iostream>
#include <map>
#include <filesystem>
using namespace std::filesystem;


bool load_bmp_24(const char* filename, Texture* out_texture) {
    //constexpr int width = TEXTURE_WIDTH;
    //constexpr int height = TEXTURE_HEIGHT;

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open BMP file.\n";
        return false;
    }

    uint8_t header[54];
    file.read(reinterpret_cast<char*>(header), 54);
    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << "Not a BMP file.\n";
        return false;
    }

    int32_t bmp_width  = *reinterpret_cast<int32_t*>(&header[18]);
    int32_t bmp_height = *reinterpret_cast<int32_t*>(&header[22]);
    int16_t bpp        = *reinterpret_cast<int16_t*>(&header[28]);
    uint32_t offset    = *reinterpret_cast<uint32_t*>(&header[10]);

    // if (bmp_width != width || bmp_height != height || bpp != 24) {
    //    std::cerr << "Unsupported BMP format (must be 32x48, 24-bit).\n";
    //    return false;
    //}
    out_texture->height = bmp_height;
    out_texture->width = bmp_width;
    out_texture->coordinate = new RGBA[bmp_height*bmp_width];

    file.seekg(offset, std::ios::beg);

    const int row_size = ((bmp_width * 3 + 3) / 4) * 4;
    std::vector<uint8_t> row(row_size);

    for (int y = 0; y < bmp_height; ++y) {
        file.read(reinterpret_cast<char*>(row.data()), row_size);
        for (int x = 0; x < bmp_width; ++x) {
            int src_index = x * 3;
            int dst_x = x;
            int dst_y = bmp_height - 1 - y;

            out_texture->coordinate[dst_y * bmp_width + dst_x] = {
                row[src_index + 2], // R
                row[src_index + 1], // G
                row[src_index + 0], // B
                255                 // A
            };
        }
    }

    return true;
};

map<string, Texture*> load_textures(){
    map<string, Texture*> texture_map;
    string texture_name;
    path directorypath = "textures";
    string filename;
    string extension;
    // To check if the directory exists or not
    if (exists(directorypath)
        && is_directory(directorypath)) {
        // Loop through each item (file or subdirectory) in
        // the directory
        for (const auto& entry : directory_iterator(directorypath)) {
            
            if (entry.path().extension().string() == ".bmp"){
                filename = entry.path().stem().string();
                //texture_name = "texture2";
                texture_map[filename] = new Texture;
                load_bmp_24(entry.path().string().c_str(), texture_map[filename]);
    
                cout << "Loaded: " << filename << endl;
            };            
        }
    }
    else {
        // Handle the case where the directory doesn't exist
        cerr << "Texture directory not found." << endl;
    }
    return texture_map;
};
