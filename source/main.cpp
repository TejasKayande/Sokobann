
#include "base.h"
#include "game.h"

#include <raylib.h>

auto main(int argc, char **argv) -> int {

    ::InitWindow(800, 600, "Trace");
    ::SetTargetFPS(60);

    Game game = { };
    game_init(&game);

    while (!::WindowShouldClose()) {

        {
            if (::IsKeyPressed(::KEY_F11)) {
                ::ToggleFullscreen();
            }
            f32 delta_time = ::GetFrameTime();
            game_update(&game, delta_time);
        }

        ::BeginDrawing();
        {
            ::ClearBackground(BLACK);
            game_render(&game);
        }
        ::EndDrawing();
    }

    ::CloseWindow();
}