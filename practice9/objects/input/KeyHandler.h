//
// Created by Bogdan Madzhuga on 16.09.2023.
//

#ifndef HOMEWORK2_KEYHANDLER_H
#define HOMEWORK2_KEYHANDLER_H


#include <SDL2/SDL_events.h>
#include <unordered_map>
#include <functional>
#include "glm/detail/type_vec1.hpp"
#include "glm/vec2.hpp"

struct ButtonEvent {
    std::function<void()> event;
    int keyCode;
    bool onPressed = false;
};

struct MouseEvent {
    std::function<void(glm::vec2 position)> event;
    int keyCode;
};

struct MouseMotionEvent {
    std::function<void(glm::vec2 position, glm::vec2 offset)> event;
};

class KeyHandler {
private:
    std::unordered_map<int, bool> keyState;
    std::vector<ButtonEvent> buttonEvents; // TODO: Replace With Map
    std::vector<MouseEvent> mouseEvents; // TODO: Replace With Map
    std::vector<MouseMotionEvent> mouseMotionEvents; // TODO: Replace With Map

public:

    void handleInputEvent(SDL_Event event);

    bool isPressed(int keyCode);

    bool isReleased(int keyCode);

    void bindOnPressedEvent(std::function<void()> event, int keyCode);

    void bindOnPressEvent(std::function<void()> event, int keyCode);

    void bindOnMouseClickEvent(std::function<void(glm::vec2 position)> event, int keyCode);
    void bindOnMouseMotionEvent(std::function<void(glm::vec2 position, glm::vec2 offset)> event);

private:

    void handleKeyboardEvent(SDL_KeyboardEvent event);

    void handleMouseEvent(SDL_MouseButtonEvent event);

    void handleMouseMotionEvent(SDL_MouseMotionEvent event);

    void notifyOnPressedEvent();

    void notifyOnPressEvent(int keyCode);

    void notifyOnMouseClickEvent(int keyCode, glm::vec2 position);

    void notifyOnMouseMotionEvent(glm::vec2 position, glm::vec2 offset);
};


#endif //HOMEWORK2_KEYHANDLER_H
