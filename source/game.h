
#if !defined(GAME_H)
#define GAME_H

#include "base.h"

#include <raylib.h>
#include <vector>

// NOTE(Tejas): Not sure yet where this belongs!!
constexpr int TILE_SIZE = 32;

struct Position {
    int x, y;
};

enum class TileType : u8 {
    None = 0,
    Wall,
    Ground
};

struct TileMap {
    u32 width;
    u32 height;
    std::vector<TileType> tiles;
};

struct Level {
    TileMap tile_map;
};

struct Player {
    Position position;
};

// NOTE(Tejas): This is temp
enum class GameCameraMode : u8 {
    Free,
    Fixed
};

struct GameCamera {
    // NOTE(Tejas): maybe instead of this we can just store the data and build
    // the Ray::Camera3D when needed
    ::Camera3D cam;
    GameCameraMode mode;

    f32 speed;
    f32 mouse_sensitivity;

    f32 yaw;
    f32 pitch;
};

struct Game {
    Player player;
    Level level;
    GameCamera camera;
};

void game_init(Game *game);
void game_update(Game *game, f32 delta_time);
void game_render(Game *game);

#endif // GAME_H