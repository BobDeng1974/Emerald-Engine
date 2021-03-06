#ifndef TERRAIN_H
#define TERRAIN_H

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
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Error.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"


class Terrain {
public:
    Terrain();
    Terrain(std::string directory, float plane_scale, float height_scale, unsigned chunk_size);
    ~Terrain();

    bool draw_me = true, clamp_textures = false, cast_shadow = true;
    glm::vec3 world_coord;
    glm::mat4 m2w_matrix, move_matrix, rot_matrix, scale_matrix;
    float bounding_sphere_radius = -1.f, scale = 1.f, height_scale = 1.f;
    static Texture *wind_map;
    static unsigned terrain_drawn;

    const std::vector<Mesh*> get_meshes();
    void load_heightmap(std::string heightmap_file, float plane_scale, float height_scale, unsigned chunk_size);
    static const std::vector<Terrain*> get_loaded_terrain();
    static uint cull_terrain(Camera &camera);
    glm::vec3 get_center_point_world();
    glm::vec3 get_center_point();
    bool point_in_terrain(float x_world, float z_world);
    unsigned cull_me(Camera* camera);
    float get_height(float x_world, float z_world);
    int get_chunk_size() {return chunk_size;}

private:
    static std::vector<Terrain*> loaded_terrain;
    int chunk_size,total_x,total_z;
    glm::vec3 last_indice,last_normal;

    GLuint VAO, EBO;
    GLuint VBO[4]; // Vertices, normals, texCoords, tangents
    std::vector<Mesh*> meshes;
    SDL_Surface* heightmap;
    glm::vec3 bounding_sphere_center;


    float get_pixel_height(int x, int y, SDL_Surface* image);
    int get_pixel_index(int x, int y, SDL_Surface* image);
    glm::vec3 get_normal(int x, int z, SDL_Surface* image, float plane_scale, float height_scale);
    glm::vec3 get_tangent(int x, int z, SDL_Surface* image, float plane_scale, float height_scale);
    void generate_bounding_sphere();
    void load_wind(std::string full_path);
};



#endif
