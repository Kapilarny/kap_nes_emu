#include <iostream>

#include "defines.h"
#include "logger.h"

#include <raylib.h>

#include "emulator.h"

int main() {
    const i32 screenW = 780 * 2;
    const i32 screenH = 480 * 2 + 100;

    InitWindow(screenW, screenH, "Kap NES Emulator");
    SetTargetFPS(60);

    emulator emu;

    while(!WindowShouldClose()) {
        emu.update();

        BeginDrawing();
        ClearBackground(DARKBLUE);

        emu.draw();

        // Draw FPS
        SetWindowTitle(TextFormat("Kap NES Emulator | FPS: %d", GetFPS()));
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
