#pragma once

#include "game.h"

class Snake : public Game {
	void _input_event(InputEvent* const p_event) override;
	void _update(double p_delta) override;
	void _draw() override;
};