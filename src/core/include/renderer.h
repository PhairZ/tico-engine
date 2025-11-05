#pragma once

#include "data.h"
#include "objects.h"

#include <string>
#include <array>
#include <vector>

class Renderer {
	struct Printable;
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
		DEFAULT // Default terminal color. (Transparent)
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
	
	void print_term(const Printable& p_printables, const Vector2I& p_coord = Vector2I(), const Color p_fg_color = WHITE, const Color p_bg_color = DEFAULT);
	void print(const Printable& p_printables, const Vector2I& p_coord = Vector2I(), const Color p_fg_color = WHITE, const Color p_bg_color = DEFAULT);

	void render_screen();

private:
	struct Printable {
		Printable(Printable&) = default;
		Printable(Printable&&) = default;
		
		Printable(char p_val):
			text(std::string({p_val, '\0'})) {}
		Printable(int p_val):
			text(std::to_string(p_val)) {}
		Printable(unsigned int p_val):
			text(std::to_string(p_val)) {}
		Printable(long p_val):
			text(std::to_string(p_val)) {}
		Printable(unsigned long p_val):
			text(std::to_string(p_val)) {}
		Printable(long long p_val):
			text(std::to_string(p_val)) {}
		Printable(unsigned long long p_val):
			text(std::to_string(p_val)) {}
		Printable(float p_float):
			text(std::to_string(p_float)) {}
		Printable(double p_float):
			text(std::to_string(p_float)) {}
		Printable(long double p_float):
			text(std::to_string(p_float)) {}

		Printable(const char* p_cstr):
			text(std::string(p_cstr)) {}
		Printable(const std::string& p_str):
			text(p_str) {}
		
		Printable(const Object& p_object):
			text(p_object.to_str()) {}
		Printable(const Vector2& p_vec):
			text(p_vec.to_str()) {}
		Printable(const Vector2I& p_vec):
			text(p_vec.to_str()) {}
		
		char operator[](int p_idx) const {
			return text[p_idx];
		}
		
		const std::string text;
	};

	struct Cell {
		Color top = DEFAULT;
		Color bottom = DEFAULT;
		std::string c = "▀";
	};

	Vector2I resolution;
	std::vector<Cell> m_display_buffer;
};
