#include "smb_game.h"
#include "sprites.h"

#include <cmath>

void SMB::_input_event(InputEvent *const p_event) {
  if (InputEventKey *key_event = dynamic_cast<InputEventKey *>(p_event)) {
    if (key_event->pressed) {
      switch (key_event->key) {
      case 'a':
        m_mario.velocity.x =
            std::lerp(m_mario.velocity.x, -Player::SPEED, 0.04);
        m_mario.flip_h = true;
        break;
      case 'd':
        m_mario.velocity.x = std::lerp(m_mario.velocity.x, Player::SPEED, 0.04);
        m_mario.flip_h = false;
        break;
      case 'A':
        m_mario.velocity.x =
            std::lerp(m_mario.velocity.x, -Player::SPEED * 2.0, 0.03);
        m_mario.flip_h = true;
        break;
      case 'D':
        m_mario.velocity.x =
            std::lerp(m_mario.velocity.x, Player::SPEED * 2.0, 0.03);
        m_mario.flip_h = false;
        break;
      case ' ':
        if (m_mario.is_on_floor()) {
          m_mario.velocity.y =
              Player::JUMP_FORCE - std::abs(m_mario.velocity.x * 4);
        }
        break;
      case 'r':
        m_mario.position = {64, 192};
        m_mario.velocity = {0, 0};
        m_cam_pos = {16, 112};
        break;
      }
    }
  }
}

void SMB::_update(double p_delta) {
  // Make camera only move forward with mario.
  if (m_mario.position.x - m_cam_pos.x > 64) {
    m_cam_pos.x = m_mario.position.x - 64;
  } else if (m_mario.position.x < m_cam_pos.x && m_mario.velocity.x < 0) {
    m_mario.velocity.x = 0;
  }

  // Damping.
  m_mario.velocity.x = std::lerp(m_mario.velocity.x, 0.0, .12);

  m_mario.move();

  // Gravity.
  if (!m_mario.is_on_floor()) {
    m_mario.velocity.y += C_GRAVITY;
  } else {
    m_mario.velocity.y = 1;
  }
  // Limit fall speed.
  if (std::abs(m_mario.velocity.y) > m_mario.TERMINAL_VELOCITY) {
    m_mario.velocity.y =
        m_mario.TERMINAL_VELOCITY * (m_mario.velocity.y > 0 ? 1 : -1);
  }
}

void SMB::draw_map(const Vector2I &p_offset) {
  for (int y = 0; y < Sprites::C_MAP_SIZE.y; y++) {
    for (int x = 0; x < Sprites::C_MAP_SIZE.x; x++) {
      int index = y * Sprites::C_MAP_SIZE.x + x;
      int sprite_index = Sprites::map_data[index];
      if (sprite_index > 0) {
        m_renderer.draw_sprite(Sprites::C_SPRITE_MAP[sprite_index],
                               p_offset + Vector2I(x * 16, y * 16));
      }
    }
  }
}

void SMB::draw_bg(const Vector2I &p_offset) {
  for (int y = 0; y < Sprites::C_BG_SIZE.y; y++) {
    for (int x = 0; x < Sprites::C_BG_SIZE.x; x++) {
      int index = y * Sprites::C_BG_SIZE.x + x;
      int sprite_index = Sprites::C_BG_DATA[index];
      if (sprite_index > 0 && sprite_index < 99) {
        m_renderer.draw_sprite(Sprites::C_BG_SPRITE_MAP[sprite_index],
                               p_offset + Vector2I(x * 16, y * 16));
      }
    }
  }
}

void SMB::_draw() {
  for (int y = 0; y < m_renderer.RESOLUTION.y; y++) {
    for (int x = 0; x < m_renderer.RESOLUTION.x; x++) {
      m_renderer.draw_pixel({x, y}, Renderer::CYAN);
    }
  }

  draw_bg(m_cam_pos * -1);
  draw_map(m_cam_pos * -1);

  const Vector2I REL_POS = m_mario.position - m_cam_pos;

  m_renderer.draw_sprite(Sprites::C_MARIO, REL_POS, m_mario.flip_h);

  m_renderer.print("FPS:" + std::to_string(1.0 / m_delta_time), {0, 0},
                   Renderer::WHITE, Renderer::RED);
  // m_renderer.print("Vel:" + m_mario.velocity.to_str(), {0, 7},
  // Renderer::WHITE); m_renderer.print("Pos:" + m_mario.position.to_str(), {0,
  // 13},
  //                  Renderer::WHITE);
  // m_renderer.print("On Floor:" +
  //                      std::string(m_mario.is_on_floor() ? "True" : "False"),
  //                  {0, 20}, Renderer::WHITE);
}
