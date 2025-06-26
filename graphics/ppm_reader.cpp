#include "ppm_reader.h"
#include "settings.h"
#include "graphic_structs.h"#include <fstream>
#include <cstdint>
#include <iostream>

bool load_bmp_24(const char* filename, Texture& out_texture) {
    constexpr int width = TEXTURE_WIDTH;
    constexpr int height = TEXTURE_HEIGHT;

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

    if (bmp_width != width || bmp_height != height || bpp != 24) {
        std::cerr << "Unsupported BMP format (must be 32x48, 24-bit).\n";
        return false;
    }

    file.seekg(offset, std::ios::beg);

    const int row_size = ((width * 3 + 3) / 4) * 4;
    uint8_t row[row_size];

    for (int y = 0; y < height; ++y) {
        file.read(reinterpret_cast<char*>(row), row_size);
        for (int x = 0; x < width; ++x) {
            int src_index = x * 3;
            int dst_x = x;
            int dst_y = height - 1 - y;

            out_texture.coordinate[dst_y * width + dst_x] = {
                row[src_index + 2], // R
                row[src_index + 1], // G
                row[src_index + 0], // B
                255                 // A
            };
        }
    }

    return true;
}
