#include <vector>
#include <deque>
#include <iostream>
#include "graphic_structs.h"
#include "vector_funcs.h"
#include "settings.h"
#include <SDL3/SDL.h>

const float FRUSTRUM_DEPTH = 3000;

vector<tri3> get_tris(Object obj){
    //Use tail end recursion to get all children
    vector<tri3> converted_coords;
    vector<tri3> results;
    if (obj.mesh.tris.size() > 0){
        for (tri3 i : obj.mesh.tris){
            //mul x y z by the obj coordinate space matrix
            converted_coords.push_back(
                tri3{add_vec3(mul_matrix3x3_vec3(obj.ObjectSpace, i.a), obj.pos),
                    add_vec3(mul_matrix3x3_vec3(obj.ObjectSpace, i.b), obj.pos),
                    add_vec3(mul_matrix3x3_vec3(obj.ObjectSpace, i.c), obj.pos)
                }
            );
        };
    };
    
    if (obj.children.size() == 0){
        return converted_coords;
    };
    for (Object child : obj.children){
        results = get_tris(child);
        converted_coords.insert(converted_coords.end(), results.begin(), results.end());
    };
    return converted_coords;
};

vector<tri3> convert_tris(deque<Object> world){
    vector<tri3> to_render;
    vector<tri3> results;
    for (Object obj : world){
        results = get_tris(obj);
        //Convert all to screenspace
        for (tri3 triangle : results){
            to_render.push_back(
                tri3{
                    project_to_screen(triangle.a),
                    project_to_screen(triangle.b),
                    project_to_screen(triangle.c)
                });
        };
    };
    return to_render;
};

vector<tri> get_uvs(Object obj){
    //Use tail end recursion to get all children
    vector<tri> converted_coords;
    vector<tri> results;
    if (obj.uv_mesh.uvs.size() > 0){
        converted_coords.insert(converted_coords.end(), obj.uv_mesh.uvs.begin(), obj.uv_mesh.uvs.end());
    };
    
    if (obj.children.size() == 0){
        return converted_coords;
    };
    for (Object child : obj.children){
        results = get_uvs(child);
        converted_coords.insert(converted_coords.end(), results.begin(), results.end());
    };
    return converted_coords;
};

void black_screen(Image* img){
    memset(img, 0, sizeof(Image));
};

void black_buffer(DepthBuffer* buffer){
    memset(buffer, 0, sizeof(DepthBuffer));
};

vec3 obj_to_world(Matrix_3x3 coordinate_space, vec3 coords){
    return vec3{0,0,0};
};

float min_bound_x(tri3& triangle){
    return floor(max(min(min(triangle.a.x,triangle.b.x), triangle.c.x), 0.0f));
};

float max_bound_x(tri3& triangle){
    return ceil(min(max(max(triangle.a.x,triangle.b.x), triangle.c.x), (float) WIDTH-1));
};

float min_bound_y(tri3& triangle){
    return floor(max(min(min(triangle.a.y,triangle.b.y), triangle.c.y), 0.0f));
};

float max_bound_y(tri3& triangle){
    return ceil(min(max(max(triangle.a.y,triangle.b.y), triangle.c.y), (float) WIDTH-1));
};

void render_tris(vector<tri3> tris, vector<tri> uvs, int num_tris, Image* image, DepthBuffer* depth_buffer, SDL_Renderer *renderer, Texture& default_texture){
    vec3 barycentric_coords;
    float depth;
    float *buffer_depth;
    for (int i=0; i<num_tris; i++) {
        tri3& current_tri = tris[i];
        //If not back-face of tri
        if (cross_product_3_vectors(current_tri.a, current_tri.b, current_tri.c) <= 0) continue;
        float min_y = min_bound_y(current_tri);
        float max_y = max_bound_y(current_tri);
        float min_x = min_bound_x(current_tri);
        float max_x = max_bound_x(current_tri);
        float y;
        float x;
        tri& base_tri = uvs[i];
        for (y=min_y;y<=max_y;++y){
            for (x=min_x;x<=max_x;++x){
                //If pixel in tri
                if (contained_in(vec3{x+0.5f,y+0.5f,1}, current_tri)){
                    //Calc depth using barycentric interpolation
                    barycentric_coords = to_barycentric(vec2{x+0.5f,y+0.5f}, current_tri);
                    depth = (barycentric_coords.x * current_tri.a.z) + (barycentric_coords.y * current_tri.b.z) + (barycentric_coords.z * current_tri.c.z);
                    //If new depth is closer to camera than old depth then overwrite old depth and colour
                    buffer_depth = &depth_buffer->depth(static_cast<size_t>(x), static_cast<size_t>(y));
                    if ((FRUSTRUM_DEPTH - depth) > *buffer_depth){
                        *buffer_depth = FRUSTRUM_DEPTH - depth;
                        vec2 texture_coordinate = barycentric_to_uv(barycentric_coords, base_tri);
                        RGBA colour = default_texture.el(rescale_int(texture_coordinate.x, 1, TEXTURE_WIDTH), rescale_int(texture_coordinate.y, 1, TEXTURE_HEIGHT));
                        image->pixel(static_cast<size_t>(x), static_cast<size_t>(y)) = colour;
                    };
                };
            };
        };
    };
    //delete texture
};

SDL_RenderPackage SDL_Init_Main(){
    SDL_RenderPackage render_storage;
    render_storage.window = NULL;
    render_storage.renderer = NULL;
    render_storage.image = new Image();
    render_storage.depth_buffer = new DepthBuffer();

    cout << "Initialisng Video Mode" << endl;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        throw;
    }
    cout << "Initialisng Renderer and Window" << endl;
    if (!SDL_CreateWindowAndRenderer("Renderer", WIDTH, HEIGHT, 0, &render_storage.window, &render_storage.renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        throw;
    }
    cout << "Creating Texture" << endl;
    render_storage.texture = SDL_CreateTexture(
        render_storage.renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );
    cout << "Changing Draw Colour" << endl;
    SDL_SetRenderDrawColorFloat(render_storage.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
    return render_storage;
};

void SDL_Exit(SDL_RenderPackage render_storage){
    SDL_DestroyTexture(render_storage.texture);
    SDL_DestroyRenderer(render_storage.renderer);
    SDL_DestroyWindow(render_storage.window);
    SDL_Quit();
}

void render(const SDL_RenderPackage& render_storage, deque<Object>& objects, Texture& default_texture){
    //Empty the depth and colour buffers
    black_screen(render_storage.image);
    black_buffer(render_storage.depth_buffer);
    if (objects.size() > 0){
        //Get list of tris to render
        vector<tri3> to_render = convert_tris(objects);
        //Get list of texture UV's
        vector<tri> uvs;
        vector<tri> uv_results;
        for (Object obj : objects){
            uv_results = get_uvs(obj);
            uvs.insert(uvs.end(), uv_results.begin(), uv_results.end());
        };
        //Draw to screen
        render_tris(to_render, uvs, to_render.size(), render_storage.image, render_storage.depth_buffer, render_storage.renderer, default_texture);   
    };
};

void present(const SDL_RenderPackage& render_storage){
    void* pixels;
    int pitch;
    /*
    //Set SDL texture to write only
    if (!SDL_LockTexture(render_storage.texture, nullptr, &pixels, &pitch)) {
        SDL_Log("SDL_LockTexture failed: %s", SDL_GetError());
        return;
    };
    //Blit custom image buffer into SDL texture
    memcpy(pixels, render_storage.image->pixels, WIDTH * HEIGHT * sizeof(RGBA));
    //Set texture back to read only
    SDL_UnlockTexture(render_storage.texture);
    */

    //Update the render texture with buffer
    SDL_UpdateTexture(render_storage.texture, nullptr, render_storage.image->pixels, WIDTH * sizeof(RGBA));
    //Clear the screen
    SDL_RenderClear(render_storage.renderer);
    //Draw the texture to the screen, flip vertically so that 0,0 is bottom left
    SDL_RenderTextureRotated(render_storage.renderer, render_storage.texture, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
    //Present buffer
    SDL_RenderPresent(render_storage.renderer);
};