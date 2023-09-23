//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef PRACTICE2_KEYHANDLER_H
#define PRACTICE2_KEYHANDLER_H


#include <SDL2/SDL_events.h>
#include <unordered_map>
#include <functional>

struct ButtonEvent {
    std::function<void()> event;
    int keyCode;
};

class KeyHandler {
private:
    std::unordered_map<int, bool> keyState;
    std::vector<ButtonEvent> events; // TODO: Replace With Map

public:

    void handleKeyboardEvent(SDL_KeyboardEvent event);

    void handleMouseEvent(SDL_MouseButtonEvent event);

    bool isPressed(int keyCode);

    bool isReleased(int keyCode);

    void bindOnPressEvent(void (*event)(), int keyCode);

private:
    void notifyOnPressEvent(int keyCode);
};


#endif //PRACTICE2_KEYHANDLER_H
