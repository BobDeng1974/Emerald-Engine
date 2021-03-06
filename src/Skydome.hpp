#ifndef SKYDOME_H
#define SKYDOME_H

#ifdef __linux__
  #include <GL/glew.h>
#endif

#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#include "Model.hpp"
#include "Utils.hpp"
#include "Error.hpp"
#include "Terrain.hpp"

class Skydome
{
public:
    float time_of_day, time_scale;

    void init();
    void draw(const Camera &camera);
    void upload_sun(const GLuint shader, const Camera &camera);
    void propagate_time(const float elapsed_time);
    void update_light_space(const Camera &camera);
    void reset_time();
    glm::mat4 get_light_space_matrix();
    void update_sun_frustum(const vec3 sun_pos, const vec3 sun_front, const vec3 sun_right);
    bool sphere_in_sun_frustum(vec3 center, float radius);

private:
    glm::vec3 sun_direction;
    glm::vec3 sun_color;
    Model *sphere; // actual dome
    GLuint shader;
    float altitude, azimuth, interp, interp_night;

    glm::vec3 zenith_color;
    glm::vec3 horizon_color;
    glm::mat4 light_view_matrix, light_space_matrix;

    void init_uniforms();
    void calculate_sky();
    void calculate_sun();

    glm::vec3 frustum_normals[5];
    float frustum_offsets[5];


    // Constant color values:
    static const glm::vec3 sun_dawn;
    static const glm::vec3 sun_noon;
    static const glm::vec3 sun_dusk;
    static const glm::vec3 sun_midnight;

    static const glm::vec3 zenith_dawn;
    static const glm::vec3 horizon_dawn;

    static const glm::vec3 zenith_noon;
    static const glm::vec3 horizon_noon;

    static const glm::vec3 zenith_dusk;
    static const glm::vec3 horizon_dusk;

    static const glm::vec3 zenith_midnight;
    static const glm::vec3 horizon_midnight;

    static const float altitude_margin;

    // Light space, used for shadow mapping
    static const glm::mat4 light_projection;
};

#endif
