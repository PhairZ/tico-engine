#include "snake_game.h"

#include <string>

struct player {
  Vector2I pos;
  Vector2I dir;
} snake;

void Snake::_init() { m_renderer.clear(); }

void Snake::_input_event(InputEvent *const p_event) {
  if (auto event = dynamic_cast<InputEventKey *>(p_event)) {
    switch (event->key) {
    case 'A':
    case 'w':
      if (!snake.dir.y) {
        snake.dir = {0, -1};
      }
      break;
    case 'B':
    case 's':
      if (!snake.dir.y) {
        snake.dir = {0, 1};
      }
      break;
    case 'C':
    case 'd':
      if (!snake.dir.x) {
        snake.dir = {1, 0};
      }
      break;
    case 'D':
    case 'a':
      if (!snake.dir.x) {
        snake.dir = {-1, 0};
      }
      break;
    }
  }
}

void Snake::_update(double p_delta) {
  static int tick = 0;
  constexpr int tps = 20;

  tick++;
  if (tick > tps) {
    tick = 0;
    snake.pos += snake.dir;
  }
}

void Snake::_draw() { m_renderer.draw_pixel(snake.pos, Renderer::GREEN); }
