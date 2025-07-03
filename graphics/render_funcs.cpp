#include <vector>
#include <deque>
#include <iostream>
#include "graphic_structs.hpp"
#include "vector_funcs.hpp"
#include "settings.hpp"
#include <SDL3/SDL.h>

vector<RenderTri> get_tris(Object obj, Object parent){
    //Use tail end recursion to get all children
    vector<RenderTri> converted_coords;
    vector<RenderTri> results;
    uint16_t size = obj.mesh.tris.size();
    if (size > 0){
        for (int i=0;i<size;i++){
            //RenderTri i : obj.mesh.tris
            //mul x y z by the obj coordinate space matrix
            converted_coords.push_back(
                RenderTri{
                    tri3{
                        add_vec3(mul_matrix3x3_vec3(parent.ObjectSpace, add_vec3(mul_matrix3x3_vec3(obj.ObjectSpace, obj.mesh.tris[i].a), obj.pos)), parent.pos),
                        add_vec3(mul_matrix3x3_vec3(obj.ObjectSpace, obj.mesh.tris[i].b), obj.pos),
                        add_vec3(mul_matrix3x3_vec3(obj.ObjectSpace, obj.mesh.tris[i].c), obj.pos)
                    },
                    obj.mesh.uvs[i],
                    obj.mesh.texture
                }
            );
        };
    };
    
    if (obj.children.size() == 0){
        return converted_coords;
    };
    for (Object child : obj.children){
        results = get_tris(child, obj);
        converted_coords.insert(converted_coords.end(), results.begin(), results.end());
    };
    return converted_coords;
};

vector<RenderTri> convert_tris(deque<Object> world, Camera camera){
    vector<RenderTri> to_render;
    vector<RenderTri> results;
    for (Object obj : world){
        Object empty = Object();
        results = get_tris(obj, empty);
        //Convert all to screenspace
        for (int i=0;i<results.size();i++){
            results[i].vertices.a = results[i].vertices.a - camera.pos;
            results[i].vertices.a = perspective_project(results[i].vertices.a);
            results[i].uv_tri.a = results[i].uv_tri.a * results[i].vertices.a.z;

            results[i].vertices.b = results[i].vertices.b - camera.pos;
            results[i].vertices.b = perspective_project(results[i].vertices.b);
            results[i].uv_tri.b = results[i].uv_tri.b * results[i].vertices.b.z;

            results[i].vertices.c = results[i].vertices.c - camera.pos;
            results[i].vertices.c = perspective_project(results[i].vertices.c);
            results[i].uv_tri.c = results[i].uv_tri.c * results[i].vertices.c.z;
        };
        to_render.insert(to_render.end(), results.begin(), results.end());
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

void render_tris(vector<RenderTri> tris, int num_tris, Image* image, DepthBuffer* depth_buffer, SDL_Renderer* renderer){
    vec3 barycentric_coords;
    float depth;
    float *buffer_depth;

    for (int i=0; i<num_tris; i++) {
        tri3& current_tri = tris[i].vertices;
        //cout << (string) current_tri << endl;
        //If not back-face of tri
        if (cross_product_3_vectors(current_tri.a, current_tri.b, current_tri.c) <= 0) continue;
        float min_y = min_bound_y(current_tri);
        float max_y = max_bound_y(current_tri);
        float min_x = min_bound_x(current_tri);
        float max_x = max_bound_x(current_tri);
        //If not contained in screen space, skip
        if (min_x > WIDTH || max_x < 0 || min_y > HEIGHT || max_y < 0) continue;
        float y;
        float x;
        float depth_percent;
        tri& base_tri = tris[i].uv_tri;
        uint16_t width = tris[i].texture->width;
        uint16_t height = tris[i].texture->height;
        for (y=min_y;y<=max_y;++y){
            for (x=min_x;x<=max_x;++x){
                //If pixel in tri
                if (contained_in(vec3{x+0.5f,y+0.5f,1}, current_tri)){
                    //Calc depth using barycentric interpolation
                    barycentric_coords = to_barycentric(vec2{x+0.5f,y+0.5f}, current_tri);
                    depth = (barycentric_coords.x * current_tri.a.z) + (barycentric_coords.y * current_tri.b.z) + (barycentric_coords.z * current_tri.c.z);
                    //If new depth is closer to camera than the old depth, overwrite old depth and colour
                    buffer_depth = &depth_buffer->depth(static_cast<size_t>(x), static_cast<size_t>(y));
                    //Theres an issue with floating point relating to depths
                    //The top pixel of the side of a cube can overwrite the top pixel of the top of an adjacent cube 
                    if (depth > *buffer_depth){
                        //depth_percent = 1-(depth-FRUSTRUM_NEAR_DEPTH)/(FRUSTRUM_DEPTH-FRUSTRUM_NEAR_DEPTH);
                        //cout << depth_percent << endl;
                        *buffer_depth = depth;
                        vec2 texture_coordinate = (barycentric_to_uv(barycentric_coords, base_tri)) / depth;
                        RGBA colour = tris[i].texture->el(rescale_int(texture_coordinate.x, 1, width), rescale_int(texture_coordinate.y, 1, height));
                        image->pixel(static_cast<size_t>(x), static_cast<size_t>(y)) = colour;// * depth_percent;
                    };
                };
            };
        };
    };
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

void render(const SDL_RenderPackage& render_storage, deque<Object>& objects, Camera camera){
    //Empty the depth and colour buffers
    black_screen(render_storage.image);
    black_buffer(render_storage.depth_buffer);
    if (objects.size() > 0){
        //Vertex Shader
        vector<RenderTri> to_render = convert_tris(objects, camera);
        //Fragment Shader
        render_tris(to_render, to_render.size(), render_storage.image, render_storage.depth_buffer, render_storage.renderer);   
    };
};

void present(const SDL_RenderPackage& render_storage){
    //Update the render texture with buffer
    SDL_UpdateTexture(render_storage.texture, nullptr, render_storage.image->pixels, WIDTH * sizeof(RGBA));
    //Clear the screen
    SDL_RenderClear(render_storage.renderer);
    //Draw the texture to the screen, flip vertically so that 0,0 is bottom left
    SDL_RenderTextureRotated(render_storage.renderer, render_storage.texture, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
    //Present buffer
    SDL_RenderPresent(render_storage.renderer);
};