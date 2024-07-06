#include <iostream>

#include "defines.h"
#include "logger.h"

#include <raylib.h>

#include "emulator.h"

int main() {
    const i32 screenW = 780 * 2;
    const i32 screenH = 480 * 2;

    InitWindow(screenW, screenH, "Kap NES Emulator");
    SetTargetFPS(60);

    emulator emu;

    while(!WindowShouldClose()) {
        emu.update();

        BeginDrawing();
        ClearBackground(DARKBLUE);

        emu.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
