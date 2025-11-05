#pragma once

#include "data.h"

#include <string>
#include <array>
#include <vector>

class Renderer {
public:
	enum Color {
		BLACK,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
		DEFAULT // Default terminal color (Transparent)
	};

	Renderer() = delete;
	explicit Renderer(const Vector2I& p_resolution);
	~Renderer();

	void set_resolution(const Vector2I& p_resolution);
	const Vector2I& get_resolution() const;

	void draw_pixel(const Vector2I& p_coord, const Color p_color);
	void draw_rect(const Vector2I& p_position, const Vector2I& p_size, Renderer::Color p_color = Renderer::WHITE);
	void draw_sprite(const char* p_sprite, const Vector2I& p_position, bool flip_h = false, bool flip_v = false, const Vector2I& p_size = { 16, 16 });
	void clear(Color p_color = Color::BLACK);
	
	void print_term(const std::string p_str, const Vector2I& p_coord = Vector2I(), const Color p_fg_color = WHITE, const Color p_bg_color = DEFAULT);
	void print(const std::string p_str, const Vector2I& p_coord = Vector2I(), const Color p_fg_color = WHITE, const Color p_bg_color = DEFAULT);

	void render_screen();

private:
	struct Cell {
		Color top = DEFAULT;
		Color bottom = DEFAULT;
		std::string c = "▀";
	};

	Vector2I resolution;
	// Double buffering
	char m_current_buf = 0;
	std::array<std::vector<Cell>, 2> m_display_buffers;
};
