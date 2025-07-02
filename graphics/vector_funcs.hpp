#pragma once
#include <string>
#include <cstdint>
#include <cstddef>
#include "graphic_structs.hpp"
#include "math_helpers.hpp"
using namespace std;

template <class arr_type>
int length_of_arr(arr_type arr[]);

vec2 get_vector(vec2 a, vec2 b);

float cross_product_z(vec2 a, vec2 b);

float dot_product(vec2 a, vec2 b);

vec3 to_barycentric(const vec2& point, const tri3& triangle);
vec2 barycentric_to_uv(const vec3& barycentric, const tri& triangle);
bool cross_product_3_vectors(const vec3& a, const vec3& b, const vec3& c);

bool all_pos(vec3& vals);

bool contained_in(const vec3& point, const tri3& bounds);

Matrix_3x3 matrix_mul_3x3(Matrix_3x3& a, Matrix_3x3& b);

vec3 mul_matrix3x3_vec3(Matrix_3x3& matrix, vec3& vec);

vec3 add_vec3(vec3 a, vec3 b);
vec3 clip_to_screen(float x, float y, float z, float w);
vec3 perspective_project(vec3& point);
vec3 orthographic_project(vec3& point);
void rotate_x(double deg, Matrix_3x3& pos);
void rotate_y(double deg, Matrix_3x3& pos);
void rotate_z(double deg, Matrix_3x3& pos);
Matrix_3x3 scale(float scalar, Matrix_3x3& matrix);
