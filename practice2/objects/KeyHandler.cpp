//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#include "KeyHandler.h"

void KeyHandler::handleKeyboardEvent(SDL_KeyboardEvent event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            this->keyState[event.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            this->keyState[event.keysym.sym] = false;
            break;
    }
}

bool KeyHandler::isPressed(SDL_KeyCode keyCode) {
    if (!this->keyState.contains(keyCode)) {
        return false;
    }

    return this->keyState[keyCode];
}

bool KeyHandler::isReleased(SDL_KeyCode keyCode) {
    return !isPressed(keyCode);
}
