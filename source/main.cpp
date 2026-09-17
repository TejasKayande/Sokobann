
#include <raylib.h>
#include <stdio.h>

auto main(int argc, char **argv) -> int {

    InitWindow(800, 600, "Trace");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
}