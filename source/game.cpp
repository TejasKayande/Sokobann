
#include "game.h"

#include <raylib.h>
#include <raymath.h>

#include <raygui.h>

#include <fstream>

// TODO(Tejas): Temp!
static ::Shader G_lighting_shader;
static int G_light_direction_location;
static int G_light_color_location;
static int G_ambient_color_location;

static void load_level_from_txt(Level *level, const char *txt) {

    std::memset(level, 0, sizeof(Level));

    std::ifstream file(txt);
    if (!file.is_open()) {
        ::TraceLog(::LOG_ERROR, "Failed to open level file: %s", txt);
        return;
    }

    file >> level->tile_map.width >> level->tile_map.height;

    level->tile_map.tiles.clear();
    level->tile_map.tiles.reserve(level->tile_map.width * level->tile_map.height);

    char tile_char;

    for (uint y = 0; y < level->tile_map.height; ++y) {
        for (uint x = 0; x < level->tile_map.width; ++x) {
            file >> tile_char;
            switch (tile_char) {
                case '#':
                    level->tile_map.tiles.push_back(TileType::Wall);
                    break;
                case '.':
                    level->tile_map.tiles.push_back(TileType::Ground);
                    break;
                case 'G':
                    level->tile_map.tiles.push_back(TileType::Goal);
                    break;
                case 'B':
                    level->blocks.push_back(Block { { x, y }, false });
                    level->tile_map.tiles.push_back(TileType::Ground);
                    break;
                case 'P':
                    level->player.position = { x, y };
                    level->tile_map.tiles.push_back(TileType::Ground);
                    break;
                default:
                    level->tile_map.tiles.push_back(TileType::None);
                    break;
            }
        }
    }
}

static void get_tile_map_dimensions_in_pxl(TileMap *tile_map, u32 *out_width, u32 *out_height) {

    *out_width = tile_map->width * TILE_SIZE;
    *out_height = tile_map->height * TILE_SIZE;
}

static TileType get_tile_type_at(TileMap *tile_map, Position pos) {

    if (pos.x < 0 || pos.x >= (i32)tile_map->width ||
        pos.y < 0 || pos.y >= (i32)tile_map->height) {
        return TileType::None;
    }

    return tile_map->tiles[pos.y * tile_map->width + pos.x];
}

static bool is_tile_walkable(TileMap *tile_map, Position pos) {

    if (pos.x < 0 || pos.x >= (i32)tile_map->width ||
        pos.y < 0 || pos.y >= (i32)tile_map->height) {
        return false;
    }

    return (get_tile_type_at(tile_map, pos) == TileType::Ground ||
            get_tile_type_at(tile_map, pos) == TileType::Goal);
}

// NOTE(Tejas): This function respects the rules of Sokobann as it will not push
// the block that cant be pushed. Returns true if the block was pushed.
static bool push_block(Level *level, Block *block) {

    int dx = block->pos.x - level->player.position.x;
    int dy = block->pos.y - level->player.position.y;

    Position next_pos = { block->pos.x + dx, block->pos.y + dy };

    if (!is_tile_walkable(&level->tile_map, next_pos)) {
        return false;
    }

    block->pos = next_pos;
    return true;
}

static bool does_tile_contain_block(Level *level, Position new_pos) {

    for (Block &block : level->blocks) {
        if (block.pos == new_pos) {
            return true;
        }
    }

    return false;
}

static bool is_level_solved(Game *game) {

    for (Block &block : game->level.blocks) {
        if (get_tile_type_at(&game->level.tile_map, block.pos) != TileType::Goal) {
            return false;
        }
    }

    return true;
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

    load_level_from_txt(&game->level, "assets/levels/level_0.txt");

    game->camera.mode = GameCameraMode::Free;
    game->camera.speed = TILE_SIZE * 8.0f;
    game->camera.mouse_sensitivity = 0.0025f;

    game->camera.yaw = 0.0f;
    game->camera.pitch = 0.0f;

    u32 level_width, level_height;
    get_tile_map_dimensions_in_pxl(&game->level.tile_map, &level_width, &level_height);

    game->camera.cam.position = { level_width / 2.0f, TILE_SIZE * 15.0f, (level_height / 2.0f) + TILE_SIZE * 15.0f };
    game->camera.cam.target = { (f32)game->level.player.position.x * TILE_SIZE, 0.0f, (f32)game->level.player.position.y * TILE_SIZE };
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
        else if (::IsKeyPressed(::KEY_S)) new_pos.y += 1;
        else if (::IsKeyPressed(::KEY_A)) new_pos.x -= 1;
        else if (::IsKeyPressed(::KEY_D)) new_pos.x += 1;

        if (new_pos.x >= 0 &&
            new_pos.x < (i32)game->level.tile_map.width &&
            new_pos.y >= 0 &&
            new_pos.y < (i32)game->level.tile_map.height) {

            // TODO(Tejas): Refactor this...
            if (is_tile_walkable(&game->level.tile_map, new_pos) && 
                !does_tile_contain_block(&game->level, new_pos)) {
                game->level.player.position = new_pos;
            }

            for (Block &block : game->level.blocks) {
                if (block.pos == new_pos) {
                    if (push_block(&game->level, &block)) game->level.player.position = new_pos;
                }
            }
        }

        if (is_level_solved(game)) load_level_from_txt(&game->level, "assets/levels/level_0.txt");
    }
}

void game_render(Game *game) {

    if (game->camera.mode == GameCameraMode::Free) {
        ::HideCursor();
        ::SetMousePosition(::GetScreenWidth() / 2, ::GetScreenHeight() / 2);
    } else {
        ::ShowCursor();
    }

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

                case TileType::Goal: {

                    tile_position.y = ground_height / 2.0f;
                    ::DrawCube(tile_position, TILE_SIZE, ground_height, TILE_SIZE, ::BROWN);

                } break;

                default: {

                } break;
            }
        }
    }

    for (const auto &block : game->level.blocks) {

        ::Vector3 block_position = { (f32)block.pos.x * TILE_SIZE, 0.0f, (f32)block.pos.y * TILE_SIZE };
        block_position.y = (ground_height / 2.0f) + (TILE_SIZE / 2.0f);

        ::DrawCube(block_position, TILE_SIZE, TILE_SIZE, TILE_SIZE, ::RED);
    }

    ::EndShaderMode();

    ::Vector3 start_pos = { (f32)game->level.player.position.x * TILE_SIZE, 0.0f, (f32)game->level.player.position.y  * TILE_SIZE };
    ::Vector3 end_pos   = { (f32)game->level.player.position.x * TILE_SIZE, (f32)TILE_SIZE, (f32)game->level.player.position.y  * TILE_SIZE };

    ::DrawCapsule(start_pos, end_pos, TILE_SIZE * 0.25f, 8, 16, ::YELLOW);

    ::EndMode3D();

    int line_gap = 20;
    ::DrawText(TextFormat("X: %.2f, Y: %.2f", game->camera.cam.position.x, game->camera.cam.position.y), 10, line_gap, 20, ::WHITE);
    line_gap += line_gap;
    ::DrawText(::TextFormat("FOV: %.2f", game->camera.cam.fovy), 10, line_gap, 20, ::WHITE);
    line_gap += line_gap;

    if (game->camera.mode == GameCameraMode::Free) {
        ::DrawText("Camera Mode: Free (C to toggle)", 10, line_gap, 20, ::YELLOW);
    } else {
        ::DrawText("Camera Mode: Fixed (C to toggle)", 10, line_gap, 20, ::YELLOW);
    }

    line_gap += line_gap;

    // if (::GuiButton(Rectangle{ 10, (f32)line_gap, 80, 40}, "Next Level")) {
    // }
}