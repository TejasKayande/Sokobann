
#if !defined(GAME_H)
#define GAME_H

#include "base.h"

#include <raylib.h>
#include <vector>

typedef ::Vector2 TileCoords;

enum class TileType : u8 {
    None = 0,
    Wall,
    Ground
};

struct TileMap {
    int width;
    int height;
    int tile_size;
    std::vector<TileType> tiles;
};

struct Level {
    TileMap tile_map;
};

struct Player {
    ::Vector2 position;
    TileCoords tile_coords;
};

// NOTE(Tejas): This is temp
enum GameCameraMode {
    CameraMode_Free,
    CameraMode_Fixed
};

struct Game {
    Player player;
    Level level;
    ::Camera3D camera;
    GameCameraMode camera_mode; // NOTE(Tejas): This is temp
};

void game_init(Game *game);
void game_update(Game *game, f32 delta_time);
void game_render(Game *game);

#endif // GAME_H