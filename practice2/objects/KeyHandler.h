//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef PRACTICE2_KEYHANDLER_H
#define PRACTICE2_KEYHANDLER_H


#include <SDL2/SDL_events.h>
#include <unordered_map>

class KeyHandler {
private:
    std::unordered_map<int, bool> keyState;

public:

    void handleKeyboardEvent(SDL_KeyboardEvent event);

    bool isPressed(SDL_KeyCode keyCode);

    bool isReleased(SDL_KeyCode keyCode);
};


#endif //PRACTICE2_KEYHANDLER_H
