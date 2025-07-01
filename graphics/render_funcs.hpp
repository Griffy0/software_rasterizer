#pragma once
#include <vector>
#include <deque>
#include "graphic_structs.hpp"
#include "vector_funcs.hpp"
#include "settings.hpp"
#include "math_helpers.hpp"
#include <SDL3/SDL.h>


vector<RenderTri> get_tris(Object obj);
vector<RenderTri> convert_tris(deque<Object> world);
void black_screen(Image* img);
void black_buffer(DepthBuffer* buffer);
vec3 obj_to_world(Matrix_3x3 coordinate_space, vec3 coords);
float min_bound_x(tri3& triangle);
float max_bound_x(tri3& triangle);
float min_bound_y(tri3& triangle);
float max_bound_y(tri3& triangle);

void render_tris(vector<RenderTri> tris, int num_tris, Image* image, DepthBuffer* depth_buffer, SDL_Renderer *renderer);
SDL_RenderPackage SDL_Init_Main();
void SDL_Exit(SDL_RenderPackage render_storage);
void render(const SDL_RenderPackage& render_storage, deque<Object>& objects);
void present(const SDL_RenderPackage& render_storage);