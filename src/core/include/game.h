#pragma once

#include "input.h"
#include "renderer.h"

class Game : public InputEventListener {
public:
	virtual ~Game() = default;

	void run();

protected:
	virtual void _init() = 0;
	virtual void _input_event(InputEvent *const p_event) override = 0;
	virtual void _update(double p_delta) = 0;
	virtual void _draw() = 0;

	bool m_running = true;

	Renderer m_renderer;
	double m_fps_cap = 30.0;			   // 0.0f for unlimited.
	double m_delta_time = 1.0 / m_fps_cap; // predict first frame's frame rate.
};
