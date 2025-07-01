#include <string>
#include <cstdint>
#include <cstddef>
#include "graphic_structs.hpp"
#include "math_helpers.hpp"
using namespace std;

template <class arr_type>
int length_of_arr(arr_type arr[]){
    int size = sizeof(arr[0]);
    return static_cast<int>(sizeof(arr) / size);
};

vec2 get_vector(vec2 a, vec2 b){
    return vec2 {b.x - a.x, b.y - a.y};
};

float cross_product_z(vec2 a, vec2 b){
    return (a.x * b.y) - (a.y * b.x);
};

float dot_product(vec2 a, vec2 b){
    return (a.x * b.x) + (a.y * b.y);
};

vec3 to_barycentric(const vec2& point, const tri3& triangle){
    float lambda1 = 
            (((triangle.b.y - triangle.c.y)*(point.x - triangle.c.x))
            + 
            ((triangle.c.x - triangle.b.x)*(point.y - triangle.c.y)))
        / 
            (((triangle.b.y - triangle.c.y)*(triangle.a.x - triangle.c.x))
            + 
            ((triangle.c.x - triangle.b.x) * (triangle.a.y - triangle.c.y)));
    float lambda2 = 
            (((triangle.c.y - triangle.a.y)*(point.x - triangle.c.x))
            + 
            ((triangle.a.x - triangle.c.x)*(point.y - triangle.c.y)))
        / 
            (((triangle.b.y - triangle.c.y)*(triangle.a.x - triangle.c.x))
            + 
            ((triangle.c.x - triangle.b.x) * (triangle.a.y - triangle.c.y)));
    float lambda3 = 1 - lambda1 - lambda2;
    return vec3{lambda1, lambda2, lambda3};
};

vec2 barycentric_to_uv(const vec3& barycentric, const tri& triangle){
    float uv_x = barycentric.x * triangle.a.x + barycentric.y * triangle.b.x + barycentric.z * triangle.c.x;
    float uv_y = barycentric.x * triangle.a.y + barycentric.y * triangle.b.y + barycentric.z * triangle.c.y;
    return vec2{uv_x, uv_y};
};

bool cross_product_3_vectors(const vec3& a, const vec3& b, const vec3& c){
    return ((b.x-a.x)*(c.y-a.y)) - ((b.y-a.y)*(c.x-a.x)) <= 0;
};

bool all_pos(vec3& vals){
    return (vals.x >= 0) && (vals.y >= 0) && (vals.z >= 0);
};

bool contained_in(const vec3& point, const tri3& bounds){
    return (cross_product_3_vectors(bounds.a, bounds.b, point) 
        && cross_product_3_vectors(bounds.b, bounds.c, point)
        && cross_product_3_vectors(bounds.c, bounds.a, point));
    //return all_pos(to_barycentric(point, bounds));
};

Matrix_3x3 matrix_mul_3x3(Matrix_3x3& a, Matrix_3x3& b){
    return Matrix_3x3{
        a.el(0,0)*b.el(0,0) + a.el(1,0)*b.el(0,1) + a.el(2,0)*b.el(0,2),
        a.el(0,0)*b.el(1,0) + a.el(1,0)*b.el(1,1) + a.el(2,0)*b.el(1,2),
        a.el(0,0)*b.el(2,0) + a.el(1,0)*b.el(2,1) + a.el(2,0)*b.el(2,2),

        a.el(0,1)*b.el(0,0) + a.el(1,1)*b.el(0,1) + a.el(2,1)*b.el(0,2),
        a.el(0,1)*b.el(1,0) + a.el(1,1)*b.el(1,1) + a.el(2,1)*b.el(1,2),
        a.el(0,1)*b.el(2,0) + a.el(1,1)*b.el(2,1) + a.el(2,1)*b.el(2,2),

        a.el(0,2)*b.el(0,0) + a.el(1,2)*b.el(0,1) + a.el(2,2)*b.el(0,2),
        a.el(0,2)*b.el(1,0) + a.el(1,2)*b.el(1,1) + a.el(2,2)*b.el(1,2),
        a.el(0,2)*b.el(2,0) + a.el(1,2)*b.el(2,1) + a.el(2,2)*b.el(2,2)
    };
};

vec3 mul_matrix3x3_vec3(Matrix_3x3& matrix, vec3& vec){
    return vec3{vec.x * matrix.el(0,0) + vec.y * matrix.el(1,0) + vec.z * matrix.el(2,0),
                vec.x * matrix.el(0,1) + vec.y * matrix.el(1,1) + vec.z * matrix.el(2,1),
                vec.x * matrix.el(0,2) + vec.y * matrix.el(1,2) + vec.z * matrix.el(2,2)};
};

vec3 add_vec3(vec3 a, vec3 b){
    return vec3{a.x + b.x, a.y + b.y, a.z + b.z};
};

vec3 project_to_screen(vec3& point){
    return point;//vec3{round(point.x)+0.5f, round(point.y)+0.5f, round(point.z)+0.5f};
};

void rotate_x(double deg, Matrix_3x3& pos){
    double theta = deg_to_rad(deg);
    Matrix_3x3 rot_matrix = Matrix_3x3 {
        1, 0, 0,
        0, (float) cos(theta), (float) sin(theta),
        0, (float) (-sin(theta)), (float) cos(theta)
    };
    pos = matrix_mul_3x3(rot_matrix, pos);
};

void rotate_y(double deg, Matrix_3x3& pos){
    double theta = deg_to_rad(deg);
    Matrix_3x3 rot_matrix = Matrix_3x3 {
        (float) cos(theta), 0, (float) -sin(theta),
        0,1,0,
        (float) (sin(theta)), 0, (float) cos(theta)
    };
    pos = matrix_mul_3x3(rot_matrix, pos);
};

void rotate_z(double deg, Matrix_3x3& pos){
    double theta = deg_to_rad(deg);
    Matrix_3x3 rot_matrix = Matrix_3x3 {
        (float) cos(theta), (float) sin(theta), 0,
        (float) (-sin(theta)), (float) cos(theta), 0,
        0, 0, 1
    };
    pos = matrix_mul_3x3(rot_matrix, pos);
};

Matrix_3x3 scale(float scalar, Matrix_3x3& matrix){ 
    return Matrix_3x3{
        matrix.el(0,0) * scalar,
        matrix.el(1,0) * scalar,
        matrix.el(2,0) * scalar,
        matrix.el(0,1) * scalar,
        matrix.el(1,1) * scalar,
        matrix.el(2,1) * scalar,
        matrix.el(0,2) * scalar,
        matrix.el(1,2) * scalar,
        matrix.el(2,2) * scalar,
    };
};
