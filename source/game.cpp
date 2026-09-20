
#include "game.h"

#include <raylib.h>
#include <raymath.h>


// NOTE(Tejas): This will be removed once we have a proper level loading system in place.
namespace Levels {

    constexpr uint NUMBER_OF_LEVELS = 3;
    int current_level_index = -1;
    Level levels[NUMBER_OF_LEVELS]  = { };
    bool initialized = false;

    void load_levels(void) {

        levels[0].tile_map.width = 13;
        levels[0].tile_map.height = 10;
        levels[0].tile_map.tiles = {
            TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
        };
        levels[0].start_pos = { 1, 1 };
        levels[0].end_pos = { 11, 8 };
        levels[0].player.position = levels[0].start_pos;


        levels[1].tile_map.width = 13;
        levels[1].tile_map.height = 10;
        levels[1].tile_map.tiles = {
            TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
        };
        levels[1].start_pos = { 1, 1 };
        levels[1].end_pos = { 11, 8 };
        levels[1].player.position = levels[1].start_pos;


        levels[2].tile_map.width = 15;
        levels[2].tile_map.height = 13;
        levels[2].tile_map.tiles = {
            TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
            TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
        };
        levels[2].start_pos = { 1, 1 };
        levels[2].end_pos = { 11, 8 };
        levels[2].player.position = levels[2].start_pos;
    }

    void load_next_level(Game *game) {

        if (!initialized) {
            load_levels();
            initialized = true;
        }

        current_level_index++;
        if (current_level_index >= NUMBER_OF_LEVELS) {
            current_level_index = 0;
        }
        game->level = levels[current_level_index];
    }
} // namespace Levels

// TODO(Tejas): Temp!
static ::Shader G_lighting_shader;
static int G_light_direction_location;
static int G_light_color_location;
static int G_ambient_color_location;

static void get_tile_map_dimensions_in_pxl(TileMap *tile_map, u32 *out_width, u32 *out_height) {

    *out_width = tile_map->width * TILE_SIZE;
    *out_height = tile_map->height * TILE_SIZE;
}

static bool is_tile_walkable(TileMap *tile_map, Position pos) {

    if (pos.x < 0 || pos.x >= (i32)tile_map->width ||
        pos.y < 0 || pos.y >= (i32)tile_map->height) {
        return false;
    }

    return tile_map->tiles[pos.y * tile_map->width + pos.x] == TileType::Ground;
}

static TileType get_tile_type_at(TileMap *tile_map, Position pos) {

    if (pos.x < 0 || pos.x >= (i32)tile_map->width ||
        pos.y < 0 || pos.y >= (i32)tile_map->height) {
        return TileType::None;
    }

    return tile_map->tiles[pos.y * tile_map->width + pos.x];
}

static void load_sample_level(Level *level) {

    level->tile_map.width = 13;
    level->tile_map.height = 10;
    level->tile_map.tiles = {
        TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Wall  , TileType::Wall  , TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall  , TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Ground, TileType::Wall,
        TileType::Wall, TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall  , TileType::Wall,
    };

    level->start_pos = { 1, 1 };
    level->end_pos = { 11, 8 };

    level->player.position = level->start_pos;
}

static void update_free_mode_camera(Game *game, f32 delta_time) {

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

static void update_fixed_mode_camera(Game *game, f32 delta_time) {

    game->camera.yaw = 0.0f;
    game->camera.pitch = -PI / 4.0f;

    TileMap *tile_map = &game->level.tile_map;

    u32 level_width, level_height;
    get_tile_map_dimensions_in_pxl(tile_map, &level_width, &level_height);

    game->camera.cam.position = { level_width / 2.0f, TILE_SIZE * 15.0f, (level_height / 2.0f) + TILE_SIZE * 15.0f };
    game->camera.cam.target = { level_width / 2.0f, 0.0f, level_height / 2.0f };
    game->camera.cam.up = { 0.0f, 1.0f, 0.0f };
    game->camera.cam.projection = CAMERA_PERSPECTIVE;
}

static void update_camera(Game *game, f32 delta_time) {

    if (::IsKeyPressed(::KEY_C)) {

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

    if (game->camera.mode == GameCameraMode::Free) update_free_mode_camera(game, delta_time);
    else update_fixed_mode_camera(game, delta_time);
}

void game_init(Game *game) {

    Levels::load_next_level(game);

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

    G_lighting_shader = ::LoadShader("assets/shader/shadow.vs", "assets/shader/shadow.fs");
    G_light_direction_location = ::GetShaderLocation(G_lighting_shader, "lightDirection");
    G_light_color_location = ::GetShaderLocation(G_lighting_shader, "lightColor");
    G_ambient_color_location = ::GetShaderLocation(G_lighting_shader, "ambientColor");

    Vector3 light_direction = { 1.0f, -1.0f, 0.5f };
    Vector3 light_color     = { 1.0f, 1.0f, 1.0f };
    Vector3 ambient_color   = { 0.25f, 0.25f, 0.25f };

    ::SetShaderValue(G_lighting_shader, G_light_direction_location, &light_direction, SHADER_UNIFORM_VEC3);
    ::SetShaderValue(G_lighting_shader, G_light_color_location, &light_color, SHADER_UNIFORM_VEC3);
    ::SetShaderValue(G_lighting_shader, G_ambient_color_location, &ambient_color, SHADER_UNIFORM_VEC3);
}

void game_update(Game *game, f32 delta_time) {

    update_camera(game, delta_time);

    if (game->camera.mode == GameCameraMode::Fixed)  {
        // TODO(Tejas): This is very temporary, just for testing purposes. 
        Position new_pos = game->level.player.position;

        if (::IsKeyPressed(::KEY_W)) new_pos.y -= 1;
        if (::IsKeyPressed(::KEY_S)) new_pos.y += 1;
        if (::IsKeyPressed(::KEY_A)) new_pos.x -= 1;
        if (::IsKeyPressed(::KEY_D)) new_pos.x += 1;

        if (new_pos.x >= 0 &&
            new_pos.x < (i32)game->level.tile_map.width &&
            new_pos.y >= 0 &&
            new_pos.y < (i32)game->level.tile_map.height) {

            if (is_tile_walkable(&game->level.tile_map, new_pos)) {
                game->level.player.position = new_pos;
            }
        }

        if (game->level.player.position == game->level.end_pos) {
            Levels::load_next_level(game);
        }
    }
}

void game_render(Game *game) {

    ::BeginMode3D(game->camera.cam);

    ::BeginShaderMode(G_lighting_shader);

    const f32 ground_height = 0.1f;
    const f32 wall_height = TILE_SIZE;

    for (uint y = 0; y < game->level.tile_map.height; ++y) {

        for (uint x = 0; x < game->level.tile_map.width; ++x) {

            // TileType tile_type = game->level.tile_map.tiles[y * game->level.tile_map.width + x];
            TileType tile_type = get_tile_type_at(&game->level.tile_map, { x, y });

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

            if (Position{ x, y } == game->level.end_pos) {
                tile_position.y = ground_height / 2.0f;
                ::DrawCube(tile_position, TILE_SIZE, ground_height, TILE_SIZE, ::RED);
            }
        }
    }

    ::EndShaderMode();

    ::Vector3 start_pos = { (f32)game->level.player.position.x * TILE_SIZE, 0.0f, (f32)game->level.player.position.y  * TILE_SIZE };
    ::Vector3 end_pos   = { (f32)game->level.player.position.x * TILE_SIZE, (f32)TILE_SIZE, (f32)game->level.player.position.y  * TILE_SIZE };

    ::DrawCapsule(start_pos, end_pos, TILE_SIZE * 0.25f, 8, 16, ::YELLOW);

    ::EndMode3D();

    ::DrawText(::TextFormat("X: %f.2, Y: %f.2, Z: %f.2", game->camera.cam.position.x, game->camera.cam.position.y, game->camera.cam.position.z), 10, 10, 20, ::WHITE);
    ::DrawText(::TextFormat("FOV: %f.2", game->camera.cam.fovy), 10, 30, 20, ::WHITE);
}