#pragma once

#include "data.h"
#include "game.h"
#include "player.h"

class SMB : public Game {
  void _init() override {}
  void _input_event(InputEvent *const p_event) override;
  void _update(double p_delta) override;
  void _draw() override;

  void draw_map(const Vector2I &p_offset = {0, 0});
  void draw_bg(const Vector2I &p_offset = {0, 0});

  Player m_mario;

  const double C_GRAVITY = 0.7;

  Vector2 m_cam_pos{0, 112};
};
