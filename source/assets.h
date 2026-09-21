#if !defined(ASSETS_H)
#define ASSETS_H

#include "base.h"

#include <raylib.h>

namespace Assets {

    ::Texture2D wall_texture;
    ::Texture2D ground_texture;
    ::Texture2D box_texture;

    ::Model wall_model;
    ::Model box_model;
    // ::Model ground_model;

    void init() {

        wall_texture   = ::LoadTexture("assets/textures/Wall.png");
        // ::Texture2D ground_texture = ::LoadTexture("assets/textures/Ground.png");
        box_texture    = ::LoadTexture("assets/textures/Box.png");

        wall_model = ::LoadModelFromMesh(::GenMeshCube(TILE_SIZE, TILE_SIZE, TILE_SIZE));
        wall_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wall_texture;

        box_model = ::LoadModelFromMesh(::GenMeshCube(TILE_SIZE, TILE_SIZE, TILE_SIZE));
        box_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = box_texture;

        // ground_model = ::LoadModelFromMesh(::GenMeshCube(TILE_SIZE, 0.1f, TILE_SIZE));
        // ground_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ground_texture;
    }

    void deinit() {

        ::UnloadModel(wall_model);
        ::UnloadModel(box_model);
        // ::UnloadModel(ground_model);

        ::UnloadTexture(wall_texture);
        ::UnloadTexture(box_texture);
        // ::UnloadTexture(ground_texture);
    }

} // namespace Assets

#endif // ASSETS_H