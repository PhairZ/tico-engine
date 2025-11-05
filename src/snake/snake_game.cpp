#include "snake_game.h"

Vector2 snake_pos;
char pk;

void Snake::_input_event(InputEvent* const p_event) {
    if (auto event = dynamic_cast<InputEventKey*>(p_event)) {
        pk = event->key;
    }
}

void Snake::_update(double p_delta) {
    
}


void Snake::_draw() {
    m_renderer.print("Pressed Key: " +pk);
}