//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef PRACTICE2_KEYHANDLER_H
#define PRACTICE2_KEYHANDLER_H


#include <SDL2/SDL_events.h>
#include <unordered_map>
#include <functional>
#include "structures/Position.h"

struct ButtonEvent {
    std::function<void()> event;
    int keyCode;
    bool onPressed = false;
};

struct MouseEvent {
    std::function<void(Position position)> event;
    int keyCode;
};

class KeyHandler {
private:
    std::unordered_map<int, bool> keyState;
    std::vector<ButtonEvent> buttonEvents; // TODO: Replace With Map
    std::vector<MouseEvent> mouseEvents; // TODO: Replace With Map

public:

    void handleKeyboardEvent(SDL_KeyboardEvent event);

    void handleMouseEvent(SDL_MouseButtonEvent event);

    bool isPressed(int keyCode);

    bool isReleased(int keyCode);

    void bindOnPressedEvent(std::function<void()> event, int keyCode);

    void bindOnPressEvent(std::function<void()> event, int keyCode);

    void bindOnMouseClickEvent(std::function<void(Position position)> event, int keyCode);

private:

    void notifyOnPressedEvent();

    void notifyOnPressEvent(int keyCode);

    void notifyOnMouseClickEvent(int keyCode, Position position);
};


#endif //PRACTICE2_KEYHANDLER_H
