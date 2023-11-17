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

    notifyOnPressedEvent();
}

void KeyHandler::handleMouseEvent(SDL_MouseButtonEvent event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            this->keyState[event.button] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            this->keyState[event.button] = false;
            notifyOnPressEvent(event.button);
            notifyOnMouseClickEvent(event.button, {event.x, event.y});
            break;
    }
}

void KeyHandler::handleMouseMotionEvent(SDL_MouseMotionEvent event) {
    if (event.type != SDL_MOUSEMOTION) {
        return;
    }

    notifyOnMouseMotionEvent({event.x, event.y}, {event.xrel, event.yrel});
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

void KeyHandler::bindOnPressEvent(std::function<void()> event, int keyCode) {
    ButtonEvent buttonEvent = ButtonEvent{event, keyCode};
    this->buttonEvents.push_back(buttonEvent);
}

void KeyHandler::bindOnPressedEvent(std::function<void()> event, int keyCode) {
    ButtonEvent buttonEvent = ButtonEvent{event, keyCode, true};
    this->buttonEvents.push_back(buttonEvent);
}

void KeyHandler::bindOnMouseClickEvent(std::function<void(glm::vec2 position)> event, int keyCode) {
    MouseEvent mouseEvent = MouseEvent{event, keyCode};
    this->mouseEvents.push_back(mouseEvent);
}

void KeyHandler::notifyOnPressEvent(int keyCode) {
    for (const ButtonEvent &event: this->buttonEvents) {
        if (event.keyCode != keyCode) {
            continue;
        }
        event.event();
    }
}

void KeyHandler::notifyOnPressedEvent() {
    for (const ButtonEvent &event: this->buttonEvents) {
        if (!event.onPressed || !keyState.contains(event.keyCode) || !keyState[event.keyCode]) {
            continue;
        }

        event.event();
    }
}

void KeyHandler::notifyOnMouseClickEvent(int keyCode, glm::vec2 position) {
    for (const MouseEvent &event: this->mouseEvents) {
        if (event.keyCode != keyCode) {
            continue;
        }
        event.event(position);
    }
}

void KeyHandler::bindOnMouseMotionEvent(std::function<void(glm::vec2, glm::vec2)> event) {
    auto mouseMotionEvent = MouseMotionEvent{event};
    this->mouseMotionEvents.push_back(mouseMotionEvent);
}


void KeyHandler::notifyOnMouseMotionEvent(glm::vec2 position, glm::vec2 offset) {
    for (int i = 0; i < this->mouseMotionEvents.size(); i++) {
        MouseMotionEvent event = mouseMotionEvents[i];
        event.event(position, offset);
    }
}

void KeyHandler::handleInputEvent(SDL_Event event) {
    handleKeyboardEvent(event.key);
    handleMouseEvent(event.button);
    handleMouseMotionEvent(event.motion);
}

