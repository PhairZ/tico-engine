#include "player.h"
#include "sprites.h"

void Player::move() {
	position += velocity;

	// === DEBUGGING ===
	a = Sprites::get_tile_at_pos(position + HEAD + Vector2(-1.0, 0.0)) || Sprites::get_tile_at_pos(position + HEAD + Vector2(1.0, 0.0));
	b = Sprites::get_tile_at_pos(position + L_FOOT);
	c = Sprites::get_tile_at_pos(position + R_FOOT);
	d = Sprites::get_tile_at_pos(position + TL_SIDE);
	e = Sprites::get_tile_at_pos(position + BL_SIDE);
	f = Sprites::get_tile_at_pos(position + TR_SIDE);
	g = Sprites::get_tile_at_pos(position + BR_SIDE);
	// =================

	// HEAD CHECK.
	if (int hit_idx = Sprites::get_tile_at_pos(position + HEAD)) {
		if (velocity.y < 0.0) {
			if (hit_idx == Sprites::MYSTERY) {
				Sprites::set_tile_at_pos(position + HEAD, Sprites::HIT_BLOCK);
			}
			position.y = ((int)position.y | 0xF) + 1;
			velocity.y = 0.0;
		}
	}

	// FEET CHECK.
	if (Sprites::get_tile_at_pos(position + L_FOOT) || Sprites::get_tile_at_pos(position + R_FOOT)) {
		if (velocity.y > 0.0) {
			on_floor = true;
			position.y = ((int)position.y & ~0xF) + 1;
			velocity.y = 0.0;
		}
	}
	else {
		on_floor = false;
	}

	// SIDE CHECK.
	if (Sprites::get_tile_at_pos(position + TL_SIDE) || Sprites::get_tile_at_pos(position + BL_SIDE)) {
		position.x += 1;
		velocity.x = 0.0;
	}
	if (Sprites::get_tile_at_pos(position + TR_SIDE) || Sprites::get_tile_at_pos(position + BR_SIDE)) {
		position.x -= 1;
		velocity.x = 0.0;
	}
}