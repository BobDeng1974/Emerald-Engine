

#include "Input.hpp"

using namespace glm;


void init_input()
{
    // Mouse init
    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void handle_keyboard_input(Camera &camera, Renderer &renderer, SDL_Window* window)
{
    SDL_Event event;
    bool handled;
    bool update_shadow = false;
    uint32 window_flags = SDL_GetWindowFlags(window);

    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if(keystate[SDL_GetScancodeFromKey(SDLK_w)]) {
        if (!camera.can_move_free()) {
            camera.toggle_free_move();
        }
        camera.set_pos(camera.get_pos() + camera.speed * renderer.get_time_diff() * camera.front);
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_s)]) {
        if (!camera.can_move_free()) {
            camera.toggle_free_move();
        }
        camera.set_pos(camera.get_pos() - camera.speed * renderer.get_time_diff() * camera.front);
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_a)]) {
        if (!camera.can_move_free()) {
            camera.toggle_free_move();
        }
        camera.set_pos(camera.get_pos() - normalize(cross(camera.front, camera.up)) * camera.speed * renderer.get_time_diff());
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_d)]) {
        if (!camera.can_move_free()) {
            camera.toggle_free_move();
        }
        camera.set_pos(camera.get_pos() + normalize(cross(camera.front, camera.up)) * camera.speed * renderer.get_time_diff());
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_LEFT)]) {
        renderer.propagate_time(false);
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_RIGHT)]) {
        renderer.propagate_time(true);
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            renderer.running = false;

        vec3 pos;
        int id, para;
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                renderer.running = false;
                break;
            case SDLK_1:
                renderer.set_mode(FORWARD_MODE);
                break;
            case SDLK_2:
                renderer.set_mode(DEFERRED_MODE);
                break;
            case SDLK_3:
                renderer.set_mode(POSITION_MODE);
                break;
            case SDLK_4:
                renderer.set_mode(NORMAL_MODE);
                break;
            case SDLK_5:
                renderer.set_mode(ALBEDO_MODE);
                break;
            case SDLK_6:
                renderer.set_mode(SPECULAR_MODE);
                break;
            case SDLK_7:
                renderer.set_mode(SSAO_MODE);
                break;
            case SDLK_8:
                renderer.set_mode(SHADOW_MODE);
                break;
            case SDLK_b:
                renderer.change_bounding_sphere_mode();
                break;
            case SDLK_k:
                renderer.set_kernel_radius(renderer.get_kernel_radius() + 0.1f);
                break;
            case SDLK_j:
                if (renderer.get_kernel_radius() > 0.2f)
                    renderer.set_kernel_radius(renderer.get_kernel_radius() - 0.1f);
                break;
            case SDLK_m:
                renderer.toggle_ssao();
                break;
            case SDLK_n:
                renderer.toggle_ssao_smoothing();
                break;
            case SDLK_LSHIFT:
                camera.toggle_speed();
                break;
            case SDLK_t:
                renderer.toggle_tweak_bar();
                break;
            case SDLK_p:
                pos = camera.get_pos();
                printf("Camera position x,y,z: %f %f %f\n", pos.x, pos.y, pos.z);
                Profiler::print_results();
                break;
            case SDLK_y:
                camera.toggle_height_lock();
                break;
            case SDLK_l:
                if ( window_flags == SDL_WINDOW_OPENGL ) {
#ifdef __APPLE__
                    if ( SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) < 0 )
                        Error::throw_warning(Error::display_init_fail, "Cant enter full screen!");
#else
                    if ( SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0 )
                        Error::throw_warning(Error::display_init_fail, "Cant enter full screen!");
#endif
                }
                else {
                    SDL_SetWindowFullscreen(window, 0);
                }
                break;
            case SDLK_f:
                if (camera.has_move_path()) {
                    camera.toggle_free_move();
                    if (camera.has_look_path() &&
                        camera.can_look_free() != camera.can_move_free()) {
                        camera.toggle_free_look();
                    }
                } else if (camera.has_look_path()) {
                    camera.toggle_free_look();
                }
                break;
            case SDLK_PERIOD:
                camera.cycle_move_anim_path(para);
                break;
            case SDLK_COMMA:
                camera.cycle_look_anim_path(para);
                break;
            case SDLK_z:
                Light::turn_off_all_lights();
                break;
            case SDLK_x:
                Light::turn_on_one_light();
                break;
            case SDLK_i:
                renderer.increase_up_interp();
                break;
            case SDLK_u:
                renderer.decrease_up_interp();
                break;
            case SDLK_v:
                renderer.toggle_show_normals();
                break;
            case SDLK_g:
                renderer.increase_grass_lod_distance();
                break;
            case SDLK_h:
                renderer.decrease_grass_lod_distance();
                break;
            case SDLK_c:
                renderer.toggle_grass_single_color();
                break;
            }
        }
    }
}

void handle_mouse_input(Camera &camera, Renderer &renderer)
{
    int dx,dy;
    unsigned int button_state;
    button_state = SDL_GetRelativeMouseState(&dx, &dy);

    int distance = std::abs(dx) + std::abs(dy);
    if (!camera.can_look_free() && distance > 2) {
        camera.toggle_free_look();
    }

    // pitch
    vec3 new_front = normalize(rotate(camera.front,
                                      -dy*camera.rot_speed*renderer.get_time_diff(),
                                      camera.right));

    // Limit pitch angle to acos(0.02) radians.
    if (abs(dot(new_front, vec3(0.f, 1.f, 0.f))) < 0.98) {
        camera.front = new_front;
    }

    // yaw
    camera.front = normalize(rotateY(camera.front,
                                     -dx*camera.rot_speed*renderer.get_time_diff()));
    camera.right = normalize(cross(camera.front, camera.up));
}
