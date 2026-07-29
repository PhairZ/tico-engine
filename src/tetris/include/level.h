#pragma once

#include "data.h"
#include "renderer.h"
#include "utils/singleton.h"

#include <array>

class Level;

class Cell {
public:
  Cell() : color(Renderer::TRANSPARENT) {}
  Cell(Renderer::Color p_color) : color(p_color) {}

  Renderer::Color color;
};

class Level {
  SINGLETON(Level) = default;

public:
  void set_cell(const Vector2I &p_pos, const Cell &p_data);
  const Cell &get_cell(const Vector2I &p_pos) const {
    return m_grid[p_pos.x + LVL_SIZE_X * p_pos.y];
  };
  int clear_line(int p_level);

  static constexpr int LVL_SIZE_X = 12;
  static constexpr int LVL_SIZE_Y = 19;

  static constexpr Renderer::Color bg = Renderer::WHITE;

private:
  std::array<Cell, (LVL_SIZE_Y + 1) * (LVL_SIZE_Y + 1)> m_grid;
};
