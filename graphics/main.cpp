#include <iostream> //cout
#include <fstream> //endl
#include <functional>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <map>
#include <SDL3/SDL.h>
#include "graphic_structs.hpp"
#include "vector_funcs.hpp"
#include "render_funcs.hpp"
#include "math_helpers.hpp"
#include "useful_template_structs.hpp"
#include "fps_counter.hpp"
#include "bmp_reader.hpp"
#include "settings.hpp"
using namespace std;

vector<Object*> physics_objects;
deque<Object> world;
double deltaTime = 0;

void move_helper(Object& obj){
    //Spin based on current velo
    rotate_z(obj.rb.velocity.z*deltaTime, obj.ObjectSpace);
    rotate_y(obj.rb.velocity.y*deltaTime, obj.ObjectSpace);
    rotate_x(obj.rb.velocity.x*deltaTime, obj.ObjectSpace);
    
    //If on window border, bounce
    if (obj.pos.x > 10)     obj.rb.velocity.x = abs(obj.rb.velocity.x) * -1;
    if (obj.pos.x < 0)      obj.rb.velocity.x = abs(obj.rb.velocity.x);
    if (obj.pos.y > 10)     obj.rb.velocity.y = abs(obj.rb.velocity.y) * -1;
    if (obj.pos.y < 0)      obj.rb.velocity.y = abs(obj.rb.velocity.y);
};

void add_rigidbody(Object& obj, float mass){
    obj.rb = Rigidbody{mass, 0, vec3{0,0,0}, vec3{0,0,0}, -98};
    physics_objects.push_back(&obj);
};

void update_physics(){
    for (Object *obj : physics_objects){
        Object& obj_reference = *obj;
        Rigidbody& rb = obj_reference.rb;
        rb.velocity = rb.velocity + (rb.accel * deltaTime);
        rb.velocity.y += (rb.gravity * deltaTime);
        //rb.velocity = rb.velocity - (rb.velocity * rb.drag * deltaTime); //not how this should work
        obj_reference.pos = obj_reference.pos + (rb.velocity * deltaTime);
    };
};

void add_cube(vec3 pos, float size, Texture* texture){
    world.push_back(Object(pos, TriangleMesh{cube.tris,cube.uvs,texture}, cube_uvs));
    world.back().ObjectSpace = scale(size, world.back().ObjectSpace);
    world.back().mesh.texture = texture;
    add_rigidbody(world.back(), 10);
    world.back().rb.AddForce(vec3{(float) (randint(600,1000)*((2*randint(0,1))-1)), (float) (randint(600,1000)*((2*randint(0,1))-1)), 0});
};

int main(){
    cout << "Creating render vars" << endl;
    SDL_RenderPackage render_storage = SDL_Init_Main();

    cout << "Initialising FPSMeter" << endl;
    Performance frame_counter = Performance();
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;

    cout << "Loading Textures" << endl;
    map<string, Texture*> texture_map = load_textures();

    cout << "Generating Cubes" << endl;
    int num_cubes = randint(3,3);
    for (int i=0;i<num_cubes;i++){
        //add_cube(vec3{(float) randint(-5, 5), (float) randint(-5, 5), (float) randint(2, 5)}, (float) (randint(75,200)/100.0f), texture_map["texture"]);
    };
    add_cube(vec3{0.5,0,4}, 1.0f, texture_map["texture"]);
    add_cube(vec3{1,0,2}, 1.0f, texture_map["texture"]);

    cout << "Started!" << endl << "=========" << endl << endl;
    
    // INIT RUNTIME VARS
    SDL_Event e;
    bool running = true;
    double runtime = 0;
    /*float mouse_x;
    float mouse_y;
    vec3 mouse_pos;
    float distance;
    float pull_strength = 300000.0f;*/
    // MAIN LOOP
    while (running && runtime < 100) {
        //Make this a check if running() func
        // Kill program when quit pressed
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            };
        };
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
        //SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
        //mouse_pos.x = WIDTH - mouse_pos.x * 0.5f;
        //mouse_pos.y = HEIGHT - mouse_pos.y;

        //cout << mouse_pos.y << endl;
        
        // UPDATE CODE GOES HERE
        //update_physics();
        for (Object& obj : world) {
            rotate_x(30*deltaTime, obj.ObjectSpace);
            //move_helper(obj);
            /*vec3 escape_vector = mouse_pos - obj.pos;
            escape_vector.z = 0;
            distance = escape_vector.magnitude() / 8;
            escape_vector.normalise();
            obj.rb.AddForce(escape_vector * deltaTime * min(obj.rb.mass * (pull_strength / pow(distance, 2)), 2000.0));
            */
            //escape_vector = escape_vector * deltaTime;
            //obj.rb.velocity = obj.rb.velocity + escape_vector;
        };

        // RENDER CODE
        render(render_storage, world);
        present(render_storage);
        update_fps(frame_counter);
        runtime += deltaTime;
    };
    
    //cout << "Drawing" << endl;
    //render(render_storage, world, texture);
    //present(render_storage);

    //cout << rescale_int(0.66f, 1, 9) << ' ' << rescale_int(0.67f, 1, 9) << endl;
    //system("pause");
    
    SDL_Exit(render_storage);
    return 0;
};