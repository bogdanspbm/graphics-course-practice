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
            notifyOnPressEvent(event.keysym.sym);
            break;
    }
}

void KeyHandler::handleMouseEvent(SDL_MouseButtonEvent event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            this->keyState[event.button] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            this->keyState[event.button] = false;
            notifyOnPressEvent(event.button);
            break;
    }
}


bool KeyHandler::isPressed(int keyCode) {
    if (!this->keyState.contains(keyCode)) {
        return false;
    }

    return this->keyState[keyCode];
}

bool KeyHandler::isReleased(int keyCode) {
    return !isPressed(keyCode);
}

void KeyHandler::bindOnPressEvent(void (*event)(), int keyCode) {
    ButtonEvent buttonEvent = ButtonEvent{event, keyCode};
    printf("%i", keyCode);
    this->events.push_back(buttonEvent);
}

void KeyHandler::notifyOnPressEvent(int keyCode) {
    for (const ButtonEvent &event: this->events) {
        if (event.keyCode != keyCode) {
            continue;
        }
        event.event();
    }
}