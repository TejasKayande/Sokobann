
#include "game.h"

#include <raylib.h>
#include <raymath.h>

static void get_tile_map_dimensions_in_pxl(TileMap *tile_map, u32 *out_width, u32 *out_height) {

    *out_width = tile_map->width * TILE_SIZE;
    *out_height = tile_map->height * TILE_SIZE;
}

void game_init(Game *game) {

    game->player.position = { 1, 1 };

    // NOTE(Tejas): Test Map
    game->level.tile_map.width = 10;
    game->level.tile_map.height = 10;
    game->level.tile_map.tiles = {
        TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Wall  , TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
    };

    game->camera.mode = GameCameraMode::Free;
    game->camera.speed = TILE_SIZE * 8.0f;
    game->camera.mouse_sensitivity = 0.0025f;

    game->camera.yaw = 0.0f;
    game->camera.pitch = 0.0f;

    game->camera.cam.position = { 0.0f, 10.0f, 10.0f };
    game->camera.cam.target = { 0.0f, 0.0f, 0.0f };
    game->camera.cam.up = { 0.0f, 1.0f, 0.0f };
    game->camera.cam.fovy = 45.0f;
    game->camera.cam.projection = CAMERA_PERSPECTIVE;
}

void game_update(Game *game, f32 delta_time) {

    if (::IsKeyPressed(KEY_C)) {

        if (game->camera.mode == GameCameraMode::Free) {
            game->camera.mode = GameCameraMode::Fixed;
        } else {
            game->camera.mode = GameCameraMode::Free;
            Vector3 direction = Vector3Subtract(game->camera.cam.target, game->camera.cam.position);
            direction = Vector3Normalize(direction);

            game->camera.yaw = atan2f(direction.x, direction.z);
            game->camera.pitch = asinf(direction.y);
        }
    }

    const f32 scroll = ::GetMouseWheelMove();
    if (scroll != 0.0f) game->camera.cam.fovy -= scroll * 5.0f;
    if (game->camera.cam.fovy < 20.0f) game->camera.cam.fovy = 20.0f;
    if (game->camera.cam.fovy > 90.0f) game->camera.cam.fovy = 90.0f;

    if (game->camera.mode == GameCameraMode::Free) {

        // TODO(Tejas): Do something about the cursor.

        ::Vector2 mouse_delta = ::GetMouseDelta();

        game->camera.yaw   -= mouse_delta.x * game->camera.mouse_sensitivity;
        game->camera.pitch -= mouse_delta.y * game->camera.mouse_sensitivity;

        const f32 pitch_limit = PI / 2.0f - 0.01f;

        if (game->camera.pitch > pitch_limit) game->camera.pitch = pitch_limit;
        if (game->camera.pitch < -pitch_limit) game->camera.pitch = -pitch_limit;

        ::Vector3 forward = {
            cosf(game->camera.pitch) * sinf(game->camera.yaw),
            sinf(game->camera.pitch),
            cosf(game->camera.pitch) * cosf(game->camera.yaw)
        };

        ::Vector3 right = {
            cosf(game->camera.yaw),
            0.0f,
            -sinf(game->camera.yaw)
        };

        ::Vector3 movement = { 0.0f, 0.0f, 0.0f };

        if (::IsKeyDown(::KEY_W)) {
            movement.x += forward.x;
            movement.z += forward.z;
        }

        if (::IsKeyDown(::KEY_S)) {
            movement.x -= forward.x;
            movement.z -= forward.z;
        }

        if (::IsKeyDown(::KEY_D)) {
            movement.x -= right.x;
            movement.z -= right.z;
        }

        if (::IsKeyDown(::KEY_A)) {
            movement.x += right.x;
            movement.z += right.z;
        }

        if (::IsKeyDown(::KEY_SPACE))      movement.y += 1.0f;
        if (::IsKeyDown(::KEY_LEFT_SHIFT)) movement.y -= 1.0f;

        if (::Vector3Length(movement) > 0.0f) {

            movement = ::Vector3Normalize(movement);
            movement = ::Vector3Scale(movement, game->camera.speed * delta_time);

            game->camera.cam.position = ::Vector3Add(game->camera.cam.position, movement);
        }

        game->camera.cam.target = ::Vector3Add(game->camera.cam.position, forward);
        game->camera.cam.up = { 0.0f, 1.0f, 0.0f };
        game->camera.cam.projection = CAMERA_PERSPECTIVE;
    }

    else {

        game->camera.yaw = 0.0f;
        game->camera.pitch = -PI / 4.0f;

        TileMap *tile_map = &game->level.tile_map;

        u32 level_width, level_height;
        get_tile_map_dimensions_in_pxl(tile_map, &level_width, &level_height);

        game->camera.cam.position = { level_width / 2.0f, TILE_SIZE * 15.0f, (level_height / 2.0f) + TILE_SIZE * 15.0f };
        game->camera.cam.target = { level_width / 2.0f, 0.0f, level_height / 2.0f };
        game->camera.cam.up = { 0.0f, 1.0f, 0.0f };
        game->camera.cam.projection = CAMERA_PERSPECTIVE;

        // TODO(Tejas): This is very temporary, just for testing purposes. 
        Position new_pos = game->player.position;

        if (::IsKeyPressed(::KEY_W)) new_pos.y -= 1;
        if (::IsKeyPressed(::KEY_S)) new_pos.y += 1;
        if (::IsKeyPressed(::KEY_A)) new_pos.x -= 1;
        if (::IsKeyPressed(::KEY_D)) new_pos.x += 1;

        if (new_pos.x >= 0 &&
            new_pos.x < (i32)tile_map->width &&
            new_pos.y >= 0 &&
            new_pos.y < (i32)tile_map->height) {

            const uint tile_index = new_pos.y * tile_map->width + new_pos.x;
            if (tile_map->tiles[tile_index] == TileType::Ground) {
                game->player.position = new_pos;
            }
        }
    }
}

void game_render(Game *game) {

    ::BeginMode3D(game->camera.cam);

    const f32 ground_height = 0.1f;
    const f32 wall_height = TILE_SIZE;

    for (uint y = 0; y < game->level.tile_map.height; ++y) {

        for (uint x = 0; x < game->level.tile_map.width; ++x) {

            TileType tile_type = game->level.tile_map.tiles[y * game->level.tile_map.width + x];

            ::Vector3 tile_position = { (f32)x * TILE_SIZE, 0.0f, (f32)y * TILE_SIZE };

            switch (tile_type) {

                case TileType::Wall: {

                    tile_position.y = (ground_height / 2.0f) + (wall_height / 2.0f);
                    ::DrawCube(tile_position, TILE_SIZE, wall_height, TILE_SIZE, ::GRAY);

                    ::DrawCubeWires(tile_position, TILE_SIZE, wall_height, TILE_SIZE, ::BLACK);

                } break;

                case TileType::Ground: {

                    tile_position.y = ground_height / 2.0f;
                    ::DrawCube(tile_position, TILE_SIZE, ground_height, TILE_SIZE, ::LIGHTGRAY);

                } break;

                default: {

                } break;
            }
        }
    }

    ::Vector3 start_pos = { (f32)game->player.position.x * TILE_SIZE, 0.0f, (f32)game->player.position.y  * TILE_SIZE };
    ::Vector3 end_pos   = { (f32)game->player.position.x * TILE_SIZE, (f32)TILE_SIZE, (f32)game->player.position.y  * TILE_SIZE };

    ::DrawCapsule(start_pos, end_pos, TILE_SIZE * 0.25f, 8, 16, ::YELLOW);

    ::EndMode3D();

    ::DrawText(::TextFormat("X: %f.2, Y: %f.2, Z: %f.2", game->camera.cam.position.x, game->camera.cam.position.y, game->camera.cam.position.z), 10, 10, 20, ::WHITE);
    ::DrawText(::TextFormat("FOV: %f.2", game->camera.cam.fovy), 10, 30, 20, ::WHITE);
}