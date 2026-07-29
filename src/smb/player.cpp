#include "player.h"
#include "sprites.h"

void Player::move() {
  constexpr int TILE_SIZE = 16;

  position.x += velocity.x;

  if (velocity.x < 0.0) { // Moving Left
    if (Sprites::get_tile_at_pos(position + TL_SIDE) ||
        Sprites::get_tile_at_pos(position + BL_SIDE)) {

      // Align left edge to the right border of the solid tile
      // Finds tile boundary and aligns position
      int tile_x = (int)(position.x + TL_SIDE.x) / TILE_SIZE;
      position.x = (tile_x + 1) * TILE_SIZE - TL_SIDE.x;
      velocity.x = 0.0;
    }
  } else if (velocity.x > 0.0) { // Moving Right
    if (Sprites::get_tile_at_pos(position + TR_SIDE) ||
        Sprites::get_tile_at_pos(position + BR_SIDE)) {

      // Align right edge to the left border of the solid tile
      int tile_x = (int)(position.x + TR_SIDE.x) / TILE_SIZE;
      position.x =
          (tile_x * TILE_SIZE) - TR_SIDE.x - 0.001f; // Avoid precision overlap
      velocity.x = 0.0;
    }
  }

  position.y += velocity.y;

  if (velocity.y < 0.0) { // Moving Up (Jumping/Head Bump)
    if (int hit_idx = Sprites::get_tile_at_pos(position + HEAD)) {
      if (hit_idx == Sprites::MYSTERY) {
        Sprites::set_tile_at_pos(position + HEAD, Sprites::HIT_BLOCK);
      }

      // Snap head to bottom edge of tile
      int tile_y = (int)(position.y + HEAD.y) / TILE_SIZE;
      position.y = (tile_y + 1) * TILE_SIZE - HEAD.y;
      velocity.y = 0.0;
    }
  } else if (velocity.y >= 0.0) { // Moving Down / Falling
    if (Sprites::get_tile_at_pos(position + L_FOOT) ||
        Sprites::get_tile_at_pos(position + R_FOOT)) {

      on_floor = true;

      // Snap feet to top edge of tile
      int tile_y = (int)(position.y + L_FOOT.y) / TILE_SIZE;
      position.y = (tile_y * TILE_SIZE) - L_FOOT.y;
      velocity.y = 0.0;
    } else {
      on_floor = false;
    }
  }
}

// void Player::move() {
//   position += velocity;
//
//   // === DEBUGGING ===
//   a = Sprites::get_tile_at_pos(position + HEAD + Vector2(-1.0, 0.0)) ||
//       Sprites::get_tile_at_pos(position + HEAD + Vector2(1.0, 0.0));
//   b = Sprites::get_tile_at_pos(position + L_FOOT);
//   c = Sprites::get_tile_at_pos(position + R_FOOT);
//   d = Sprites::get_tile_at_pos(position + TL_SIDE);
//   e = Sprites::get_tile_at_pos(position + BL_SIDE);
//   f = Sprites::get_tile_at_pos(position + TR_SIDE);
//   g = Sprites::get_tile_at_pos(position + BR_SIDE);
//   // =================
//
//   // HEAD CHECK.
//   if (int hit_idx = Sprites::get_tile_at_pos(position + HEAD)) {
//     if (velocity.y < 0.0) {
//       if (hit_idx == Sprites::MYSTERY) {
//         Sprites::set_tile_at_pos(position + HEAD, Sprites::HIT_BLOCK);
//       }
//       position.y = ((int)position.y | 0xF) + 1;
//       velocity.y = 0.0;
//     }
//   }
//
//   // FEET CHECK.
//   if (Sprites::get_tile_at_pos(position + L_FOOT) ||
//       Sprites::get_tile_at_pos(position + R_FOOT)) {
//     if (velocity.y > 0.0) {
//       on_floor = true;
//       position.y = ((int)position.y & ~0xF) + 1;
//       velocity.y = 0.0;
//     }
//   } else {
//     on_floor = false;
//   }
//
//   // SIDE CHECK.
//   if (Sprites::get_tile_at_pos(position + TL_SIDE) ||
//       Sprites::get_tile_at_pos(position + BL_SIDE)) {
//     position.x += 1;
//     velocity.x = 0.0;
//   }
//   if (Sprites::get_tile_at_pos(position + TR_SIDE) ||
//       Sprites::get_tile_at_pos(position + BR_SIDE)) {
//     position.x -= 1;
//     velocity.x = 0.0;
//   }
// }
