//
// Created by user on 07.07.2024.
//

#ifndef GAMEPAD_H
#define GAMEPAD_H
#include <xinput.h>

#include "defines.h"

class gamepad {
public:
    gamepad();

    bool is_connected();
    bool refresh();
    bool is_pressed(WORD button);
    XINPUT_STATE* get_state();
private:
    XINPUT_STATE state;

    f32 deadzoneX = 0.05f;
    f32 deadzoneY = 0.02f;

    f32 leftStickX = 0.0f;
    f32 leftStickY = 0.0f;
    f32 rightStickX = 0.0f;
    f32 rightStickY = 0.0f;
    i32 controller_id;
};

#endif //GAMEPAD_H
