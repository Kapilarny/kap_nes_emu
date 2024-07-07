//
// Created by user on 07.07.2024.
//

#include "gamepad.h"

#include <minwinbase.h>
#include <windows.h>

#include <cmath>

gamepad::gamepad() {
    ZeroMemory(&state, sizeof(XINPUT_STATE));

}

bool gamepad::is_connected() {
    controller_id = -1;
    for(DWORD i = 0; i < XUSER_MAX_COUNT && controller_id == -1; i++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        if(XInputGetState(i, &state) == ERROR_SUCCESS) {
            controller_id = i;
        }
    }

    return controller_id != -1;
}

bool gamepad::refresh() {
    if(controller_id == -1) is_connected();

    if(controller_id != -1) {
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        if(XInputGetState(controller_id, &state) != ERROR_SUCCESS) {
            controller_id = -1;
            return false;
        }

        f32 normLX = fmaxf(-1, (f32)state.Gamepad.sThumbLX / 32767);
        f32 normLY = fmaxf(-1, (f32)state.Gamepad.sThumbLY / 32767);

        leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
        leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

        if(deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
        if(deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

        f32 normRX = fmaxf(-1, (f32)state.Gamepad.sThumbRX / 32767);
        f32 normRY = fmaxf(-1, (f32)state.Gamepad.sThumbRY / 32767);

        rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
        rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

        if(deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
        if(deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);

        return true;
    }

    return false;
}

bool gamepad::is_pressed(WORD button) {
    return (state.Gamepad.wButtons & button) != 0;
}

XINPUT_STATE* gamepad::get_state() {
    return &state;
}
