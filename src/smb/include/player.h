#pragma once

#include "objects.h"
#include "data.h"

class Player : Object {
public:
	//bool colliding();
	void move();
	bool is_on_floor() { return on_floor; }

	Vector2 position{ 40, 192 };
	Vector2 velocity{ 0, 0 };
	bool flip_h = false;

	static constexpr double SPEED = 10.0;
	static constexpr double JUMP_FORCE = -12.0;
	static constexpr double TERMINAL_VELOCITY = 7.0;

private:

	bool on_floor = false;

	// debug
	bool a = false, b = false, c = false, d = false, e = false, f = false, g = false;

	// colliders
	static constexpr Vector2I HEAD = { 8, 0 }; // A
	static constexpr Vector2I L_FOOT = { 3, 16 }; // B
	static constexpr Vector2I R_FOOT = { 12, 16 }; // C
	static constexpr Vector2I TL_SIDE = { 2, 4 }; // D
	static constexpr Vector2I BL_SIDE = { 2, 12 }; // E
	static constexpr Vector2I TR_SIDE = { 14, 4 }; // F
	static constexpr Vector2I BR_SIDE = { 14, 12 }; // G
};