#include <string>
#include "graphic_structs.h"
#include "vector_funcs.h"
#include "settings.h"
using namespace std;

vec2::operator string() const {
    return "(" + to_string(x) + ", " + to_string(y) + ")";
};

vec3::operator string() const {
    return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
};

void vec2::normalise(){
    float euclid_norm = static_cast<float>(sqrt(pow(x, 2) + pow(y, 2)));
    x /= euclid_norm;
    y /= euclid_norm;
};

void vec3::normalise(){
    float euclid_norm = static_cast<float>(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
    x /= euclid_norm;
    y /= euclid_norm;
    z /= euclid_norm;
};

float vec3::magnitude(){
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
};

vec3 vec3::operator + (vec3 other) const {
    return vec3{x+other.x, y+other.y, z+other.z};
};

vec3 vec3::operator + (float scalar) const {
    return vec3{x + scalar, y + scalar, z + scalar};
};

vec3 vec3::operator - (vec3 other) const {
    return vec3{x-other.x, y-other.y, z-other.z};
};

vec3 vec3::operator - (float scalar) const {
    return vec3{x - scalar, y - scalar, z - scalar};
};

vec3 vec3::operator * (vec3 other) const {
    return vec3{x * other.x, y * other.y, z * other.z};
};

vec3 vec3::operator * (float scalar) const {
    return vec3{x * scalar, y * scalar, z * scalar};
};

vec3 vec3::operator / (vec3 other) const {
    return vec3{x / other.x, y / other.y, z / other.z};
};

vec3 vec3::operator / (float scalar) const {
    return vec3{x / scalar, y / scalar, z / scalar};
};

RGBA RGBA::operator - (float scalar){
    //uint8_t sub = static_cast<uint8_t>(scalar);
    return RGBA {static_cast<uint8_t>(max(r-scalar, 0.0f)), static_cast<uint8_t>(max(g-scalar, 0.0f)), static_cast<uint8_t>(max(b-scalar, 0.0f)), a};
};

/*void tri::order(){
    vec2 point_2=b; 
    vec2 point_3=c;
    vec2 ab = get_vector(a, point_2);
    ab.normalise();
    vec2 ac = get_vector(a, point_3);
    ac.normalise();
    if (cross_product_z(ab, ac) > 0){
        b = point_3;
        c = point_2;
    }
    else {
        b = point_2;
        c = point_3;
    }; 
};*/

tri::tri(vec2 a_point, vec2 b_point, vec2 c_point){
    a = a_point;
    b = b_point;
    c = c_point;
};

tri::operator string () const {
    return (string) a + ", " + (string) b + ", " + (string) c;
};

tri3::operator string() const {
    return "(" + (string) a + ", " + (string) b + ", " + (string) c + ")";
};

void Rigidbody::AddForce(vec3 force){
    //accel = accel + (force / mass);
    velocity = velocity + (force / mass);
};

Object::Object(vec3 position, TriangleMesh tri_mesh, UVMesh uvs, Matrix_3x3 space){
    pos = position;
    ObjectSpace = space;
    mesh = tri_mesh;
    uv_mesh = uvs;
};