
#include "game.h"
#include <raylib.h>
#include <raymath.h>

void game_init(Game *game) {

    // NOTE(Tejas): right now our world coords and tile coords are same because
    // tile size is 1.0f, but this will change when we change the tile size to
    // something else
    game->player.position     = { 1.0f, 1.0f };
    game->player.tile_coords  = { 0.0f, 0.0f };

    game->level.tile_map.width = 10;
    game->level.tile_map.height = 10;
    game->level.tile_map.tile_size = 1.0f;
    game->level.tile_map.tiles.resize(game->level.tile_map.width * game->level.tile_map.height, TileType::Ground);

    for (int x = 2; x < game->level.tile_map.width; ++x) {
        game->level.tile_map.tiles[x] = TileType::Wall; // Top row
        game->level.tile_map.tiles[(game->level.tile_map.height - 1) * game->level.tile_map.width + x] = TileType::Wall; // Bottom row
    }
    for (int y = 2; y < game->level.tile_map.height; ++y) {
        game->level.tile_map.tiles[y * game->level.tile_map.width] = TileType::Wall; // Left column
        game->level.tile_map.tiles[y * game->level.tile_map.width + (game->level.tile_map.width - 1)] = TileType::Wall; // Right column
    }

    int level_width = game->level.tile_map.width * game->level.tile_map.tile_size;
    int level_height = game->level.tile_map.height * game->level.tile_map.tile_size;

    game->camera.position = { game->player.position.x, 10.0f, game->player.position.y + 10.0f };
    game->camera.target   =  { level_width / 2.0f, 0.0f, level_height / 2.0f };
    game->camera.up = { 0.0f, 1.0f, 0.0f };
    game->camera.fovy = 45.0f;
    game->camera.projection = CAMERA_PERSPECTIVE;

    game->camera_mode = CameraMode_Free;
}

void game_update(Game *game, f32 delta_time) {

    // TODO(Tejas): remove these from here!!!
    static f32 camera_yaw = 0.0f;
    static f32 camera_pitch = 0.0f;

    if (::IsKeyPressed(KEY_C)) {

        game->camera_mode = (game->camera_mode == CameraMode_Free) ? CameraMode_Fixed : CameraMode_Free;
    }

    if (game->camera_mode == CameraMode_Free) {

        const f32 camera_speed = 8.0f;
        const f32 mouse_sensitivity = 0.0025f;

        const f32 scroll = ::GetMouseWheelMove();
        if (scroll != 0.0f) game->camera.fovy -= scroll * 5.0f;

        Vector2 mouse_delta = ::GetMouseDelta();

        camera_yaw -= mouse_delta.x * mouse_sensitivity;
        camera_pitch -= mouse_delta.y * mouse_sensitivity;

        const f32 pitch_limit = PI / 2.0f - 0.01f;

        if (camera_pitch > pitch_limit) camera_pitch = pitch_limit;
        if (camera_pitch < -pitch_limit) camera_pitch = -pitch_limit;


        Vector3 forward = {
            cosf(camera_pitch) * sinf(camera_yaw),
            sinf(camera_pitch),
            cosf(camera_pitch) * cosf(camera_yaw)
        };

        Vector3 right = { cosf(camera_yaw), 0.0f, -sinf(camera_yaw) };


        Vector3 movement = { 0.0f, 0.0f, 0.0f };

        if (::IsKeyDown(KEY_W)) {
            movement.x += forward.x;
            movement.z += forward.z;
        }

        if (::IsKeyDown(KEY_S)) {
            movement.x -= forward.x;
            movement.z -= forward.z;
        }

        if (::IsKeyDown(KEY_D)) {
            movement.x -= right.x;
            movement.z -= right.z;
        }

        if (::IsKeyDown(KEY_A)) {
            movement.x += right.x;
            movement.z += right.z;
        }

        if (::IsKeyDown(KEY_SPACE)) {
            movement.y += 1.0f;
        }

        if (::IsKeyDown(KEY_LEFT_SHIFT)) {
            movement.y -= 1.0f;
        }

        if (::Vector3Length(movement) > 0.0f) {
            movement = ::Vector3Normalize(movement);

            game->camera.position.x += movement.x * camera_speed * delta_time;
            game->camera.position.y += movement.y * camera_speed * delta_time;
            game->camera.position.z += movement.z * camera_speed * delta_time;
        }

        game->camera.target = ::Vector3Add(game->camera.position, forward);
    }

    else {

        // TODO(Tejas): need to put these random values somewhere in a config or something...

        const TileMap *tile_map = &game->level.tile_map;

        const f32 level_width = tile_map->width * tile_map->tile_size;
        const f32 level_height = tile_map->height * tile_map->tile_size;

        game->camera.position = { level_width / 2.0f, 10.0f, (level_height / 2.0f)  + 10.0f};
        game->camera.target   =  { level_width / 2.0f, 0.0f, level_height / 2.0f };
        game->camera.up       = { 0.0f, 1.0f, 0.0f };
        game->camera.projection = CAMERA_PERSPECTIVE;

        const f32 scroll = ::GetMouseWheelMove();
        if (scroll != 0.0f) game->camera.fovy -= scroll * 5.0f;

        // TODO(Tejas): This is very temporary, just for testing purposes. 
        ::Vector2 new_pos = game->player.position;
        if (::IsKeyPressed(KEY_W)) new_pos.y -= 1.0f;
        if (::IsKeyPressed(KEY_S)) new_pos.y += 1.0f;
        if (::IsKeyPressed(KEY_A)) new_pos.x -= 1.0f;
        if (::IsKeyPressed(KEY_D)) new_pos.x += 1.0f;

        if (game->level.tile_map.tiles[new_pos.y * level_width + new_pos.x] == TileType::Ground) {
            game->player.position = new_pos;
        }
    }
}

void game_render(Game *game) {

    ::BeginMode3D(game->camera);

    const f32 ground_height = 0.1f;
    const f32 wall_height = 1.0f;

    for (int y = 0; y < game->level.tile_map.height; ++y) {

        for (int x = 0; x < game->level.tile_map.width; ++x) {

            TileType tile_type = game->level.tile_map.tiles[y * game->level.tile_map.width + x];

            Vector3 tile_position = {
                (f32)x * game->level.tile_map.tile_size,
                0.0f,
                (f32)y * game->level.tile_map.tile_size
            };

            switch (tile_type) {
                case TileType::Wall: {
                    tile_position.y = ground_height / 2.0f + wall_height / 2.0f;

                    ::DrawCube(
                        tile_position,
                        (f32)game->level.tile_map.tile_size,
                        wall_height,
                        (f32)game->level.tile_map.tile_size,
                        ::GRAY
                    );
                } break;

                case TileType::Ground: {

                    tile_position.y = ground_height / 2.0f;

                    ::DrawCube(
                        tile_position,
                        (f32)game->level.tile_map.tile_size,
                        ground_height,
                        (f32)game->level.tile_map.tile_size,
                        ::LIGHTGRAY
                    );
                } break;

                default: {

                } break;
            }
        }
    }

    ::Vector3 start_pos = { game->player.position.x, 0.0f, game->player.position.y };
    ::Vector3 end_pos   = { game->player.position.x, 1.0f, game->player.position.y };

    ::DrawCapsule(start_pos, end_pos, 0.25f, 8, 16, ::YELLOW);

    ::EndMode3D();
}