#pragma once
#include "graphic_structs.hpp"

Matrix_3x3 identity3x3 = {1,0,0, 0,1,0, 0,0,1};

Texture default_texture = {3, 3, new RGBA[9]{
    RGBA{0,0,255, 255}, RGBA{255,0,0, 255}, RGBA{255, 0, 0, 255},
    RGBA{255,0,0, 255}, RGBA{0,0,255, 255}, RGBA{0, 0, 255, 255},
    RGBA{0,0,255, 255}, RGBA{255,0,0, 255}, RGBA{255, 0, 0, 255}}
};
/*
TriangleMesh triangle_mesh = {{
    tri3{
        vec3{-50, 50, 1}, 
        vec3{50, -50, 1}, 
        vec3{-50, -50, 1}}},
    default_texture};

TriangleMesh square = {{
    // Front
    tri3{
        vec3{-50, 50, -50}, 
        vec3{50, 50, -50},
        vec3{-50, -50, -50}
        },
    tri3{
        vec3{-50, -50, -50}, 
        vec3{50, 50, -50}, 
        vec3{50, -50, -50}}}};

UVMesh square_uvs = {{
    // Front
    tri{vec2{0,0},vec2{1,0},vec2{0,1}}, 
    tri{vec2{0,1},vec2{1,0},vec2{1,1}}}};
*/

TriangleMesh cube = {
    {
        // Front
        {0, 1, 2},
        {2, 1, 3}, 

        // Back
        {4, 5, 6},
        {6, 5, 7},

        // Right
        {1, 4, 3},
        {3, 4, 6},

        // Bottom
        {2, 3, 7},
        {7, 3, 6},

        // Top
        {5, 4, 0},
        {0, 4, 1},
        
        //Left
        {5, 0, 7},
        {7, 0, 2}

    },
    {
        // Front
        vec3{-0.5f, 0.5f, -0.5f},
        vec3{0.5f, 0.5f, -0.5f},
        vec3{-0.5f, -0.5f, -0.5f},
        vec3{0.5f, -0.5f, -0.5f},

        // Back
        vec3{0.5f, 0.5f, 0.5f},
        vec3{-0.5f, 0.5f, 0.5f},
        vec3{0.5f, -0.5f, 0.5f},
        vec3{-0.5f, -0.5f, 0.5f}},
    {
        // Front
        tri{vec2{0,0},vec2{0.49f,0},vec2{0,0.33f}}, 
        tri{vec2{0,0.33f},vec2{0.49f,0},vec2{0.49f,0.33f}}, 

        // Rear
        tri{vec2{0,0.67f},vec2{0.49f,0.67f},vec2{0,1}}, 
        tri{vec2{0,1},vec2{0.49f,0.67f},vec2{0.49f,1}}, 

        // Right
        tri{vec2{0.5f,0},vec2{1,0},vec2{0.5f,0.33f}}, 
        tri{vec2{0.5f,0.33f},vec2{1,0},vec2{1,0.33f}}, 

        // Bottom
        tri{vec2{0,0.334f},vec2{0.49f,0.334f},vec2{0,0.66f}}, 
        tri{vec2{0,0.66f},vec2{0.49f,0.334f},vec2{0.49f,0.66f}}, 

        // Top
        tri{vec2{0.5f,0.334f},vec2{1,0.334f},vec2{0.5f,0.66f}}, 
        tri{vec2{0.5f,0.66f},vec2{1,0.334f},vec2{1,0.66f}}, 

        // Left
        tri{vec2{0.5f,0.67f},vec2{1,0.67f},vec2{0.5f,1}}, 
        tri{vec2{0.5f,1},vec2{1,0.67f},vec2{1,1}}
    },
    &default_texture
};

UVMesh cube_uvs = {{
    // Front
    tri{vec2{0,0},vec2{0.49f,0},vec2{0,0.33f}}, 
    tri{vec2{0,0.33f},vec2{0.49f,0},vec2{0.49f,0.33f}}, 

    // Right
    tri{vec2{0.5f,0},vec2{1,0},vec2{0.5f,0.33f}}, 
    tri{vec2{0.5f,0.33f},vec2{1,0},vec2{1,0.33f}}, 

    // Bottom
    tri{vec2{0,0.334f},vec2{0.49f,0.334f},vec2{0,0.66f}}, 
    tri{vec2{0,0.66f},vec2{0.49f,0.334f},vec2{0.49f,0.66f}}, 

    // Top
    tri{vec2{0.5f,0.334f},vec2{1,0.334f},vec2{0.5f,0.66f}}, 
    tri{vec2{0.5f,0.66f},vec2{1,0.334f},vec2{1,0.66f}}, 

    // Rear
    tri{vec2{0,0.67f},vec2{0.49f,0.67f},vec2{0,1}}, 
    tri{vec2{0,1},vec2{0.49f,0.67f},vec2{0.49f,1}}, 

    // Left
    tri{vec2{0.5f,0.67f},vec2{1,0.67f},vec2{0.5f,1}}, 
    tri{vec2{0.5f,1},vec2{1,0.67f},vec2{1,1}}
}};