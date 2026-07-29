#include "level.h"

void Level::set_cell(const Vector2I &p_pos, const Cell &p_data) {
  m_grid[p_pos.x + LVL_SIZE_X * p_pos.y] = p_data;
}

int Level::clear_line(int p_lvl) {
  int first_cleared = 0;
  int cleared_lines = 0;

  // Iterate over all possible levels the block touched.
  for (int y = std::max(p_lvl - 1, 1); y < std::min(p_lvl + 3, (int)LVL_SIZE_Y);
       y++) {
    bool cleared = true;
    for (int x = 1; x < LVL_SIZE_X; x++) {
      // If all cells on Y level are not empty, then the level is cleared.
      if (get_cell({x, y}).color == Renderer::TRANSPARENT) {
        cleared = false;
        break;
      }
    }

    // Clear only consecutive cleared lines.
    if (cleared) {
      first_cleared = y;
      cleared_lines++;
      for (int x = 1; x < LVL_SIZE_X; x++) {
        set_cell({x, y}, Cell());
      }
    } else if (cleared_lines > 0) {
      break;
    }
  }

  if (cleared_lines == 0) {
    return 0;
  }

  // Shift all levels from the top to the lowest cleared level `cleared_lines`
  // levels down.
  for (int y = first_cleared; y > cleared_lines; y--) {
    for (int x = 1; x < LVL_SIZE_X; x++) {
      set_cell({x, y}, get_cell({x, y - cleared_lines}));
    }
  }

  // Clear consecutive lines, shift down, rinse and repeat until the block has
  // no cleared levels.
  return cleared_lines + clear_line(p_lvl);
}
