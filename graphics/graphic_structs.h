#pragma once
#include <string>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <SDL3/SDL.h>
#include "settings.h"
using namespace std;

class vec2 {
    public:
        float x;
        float y;
        operator string() const;
        void normalise();
};

struct vec3 {
    float x;
    float y;
    float z;
    operator string() const;
    void normalise();
    float magnitude();
    vec3 operator + (vec3 other) const;
    vec3 operator + (float scalar) const;
    vec3 operator - (vec3 other) const;
    vec3 operator - (float scalar) const;
    vec3 operator * (vec3 other) const;
    vec3 operator * (float scalar) const;
    vec3 operator / (vec3 other) const;
    vec3 operator / (float scalar) const;
};

struct RGB {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};
struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;    
    uint8_t a;
    RGBA operator - (float scalar);
};

class tri {
    public: 
        vec2 a;
        vec2 b;
        vec2 c;
        tri(vec2 a_point=vec2 {0,0}, vec2 b_point=vec2 {0,0}, vec2 c_point=vec2 {0,0});
        //void order();
        operator string () const;
};

struct Texture {
    uint16_t height;
    uint16_t width;
    RGBA* coordinate; 
    RGBA el(int w, int h) { return coordinate[h * width + w]; };
};

struct tri3 {
    vec3 a;
    vec3 b;
    vec3 c;
    operator string() const;
};

struct TriIndices{
    int indices[3];
};

struct TriangleMesh{
    /*int vertex_count;
    vec3 *vertices;

    int tri_count;
    TriIndices *tri_indices;*/
    vector<tri3> tris;
    vector<tri> uvs;
    Texture* texture;
    
};

struct UVMesh{
    vector<tri> uvs;
};

struct Matrix_3x3{
    float table[9]; 
    float& el(size_t w, size_t h) { return table[h * 3 + w]; };
};

class poly {
    public:
        int sides;
        vec3 *vertices;
        poly(int num_sides){
            sides = num_sides;
            vertices = new vec3[sides];
        };
};

struct Image {
    alignas(32) RGBA pixels[WIDTH * HEIGHT * 3]; 
    RGBA& pixel(size_t w, size_t h) { return pixels[h * WIDTH + w]; };  
};

struct DepthBuffer {
    float depths[WIDTH * HEIGHT * 3]; 
    float& depth(size_t w, size_t h) { return depths[h * WIDTH + w]; };  
};

class Component{
    uint8_t type;
    Component();
};

class Rigidbody {
    public:
        float mass; //kg
        float drag;
        vec3 velocity; //m/s
        vec3 accel; //m/s/s
        float gravity = 0;
        void AddForce(vec3 force); //N
};

//swap this to use quaternions later
class Object {  
    public:
        vec3 pos;
        Matrix_3x3 ObjectSpace; //ObjectSpace
        TriangleMesh mesh;
        UVMesh uv_mesh;
        vector<Object> children;
        vector<Component> components;
        Rigidbody rb;
        Object(vec3 position, TriangleMesh tri_mesh = {vector<tri3>(), vector<tri>()}, UVMesh uvs = {vector<tri>()}, Matrix_3x3 space = Matrix_3x3{1,0,0, 0,1,0, 0,0,1});
};

struct SDL_RenderPackage {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    Image* image;
    DepthBuffer* depth_buffer;
};


