#include "renderer.h"
#include "font.h"
#include "color.h"

// #include <locale>
#include <algorithm>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#define null_device "NUL"
#else
#include <unistd.h>
#define null_device "/dev/null"
#endif // _WIN32

Renderer::Renderer(const Vector2I& p_resolution) :
	resolution(p_resolution),
	m_display_buffers{
		std::vector<Cell>(p_resolution.x * (p_resolution.y / 2)),
		std::vector<Cell>(p_resolution.x * (p_resolution.y / 2))
	} {

	FILE* nullFile = fopen(null_device, "w");
	if (nullFile) {
		fflush(stderr);
		(void)dup2(fileno(nullFile), fileno(stderr));
		fclose(nullFile);
	}
	
	std::cout << "\033[?25l"; // Hide Cursor.
	// std::setlocale(LC_ALL, "en_US.UTF-8");
	std::iostream::sync_with_stdio(false);
	std::cin.tie(nullptr);
}

Renderer::~Renderer(){
	std::cout << "\033[?25h"; // Show Cursor.
}

void Renderer::set_resolution(const Vector2I& p_resolution) {
	resolution = p_resolution;
	m_display_buffers[0].resize(p_resolution.x * (p_resolution.y / 2));
	m_display_buffers[1].resize(p_resolution.x * (p_resolution.y / 2));
	m_current_buf = 0;
}

const Vector2I& Renderer::get_resolution() const {
	return resolution;
}

void Renderer::draw_pixel(const Vector2I& p_pos, const Color p_color) {
	if (p_pos.x < 0 || p_pos.x >= resolution.x || p_pos.y < 0 || p_pos.y >= resolution.y) {
		return;
	}

	auto& current_buf = m_display_buffers[m_current_buf];
	current_buf[p_pos.x + resolution.x * (p_pos.y / 2)].c = "▀";
	if (p_pos.y & 1) {
		current_buf[p_pos.x + resolution.x * (p_pos.y / 2)].bottom = p_color;
	} else {
		current_buf[p_pos.x + resolution.x * (p_pos.y / 2)].top = p_color;
	}
}

void Renderer::draw_rect(const Vector2I& p_position, const Vector2I& p_size, Renderer::Color p_color) {
	for (int y = 0; y < p_size.y; y++) {
		for (int x = 0; x < p_size.x; x++) {
			draw_pixel(p_position + Vector2I(x, y), p_color);
		}
	}
}

void Renderer::draw_sprite(const char* p_sprite, const Vector2I& p_position, bool flip_h, bool flip_v, const Vector2I& p_size) {
	for (int y = 0; y < p_size.y; y++) {
		for (int x = 0; x < p_size.x; x++) {
			int index = y * p_size.x + x;
			Renderer::Color color = static_cast<Renderer::Color>(p_sprite[index]);
			if (color != Renderer::DEFAULT) {
				Vector2I pos = p_position + Vector2I(flip_h ? p_size.x - 1 - x : x, flip_v ? p_size.y - 1 - y : y);
				draw_pixel(pos, color);
			}
		}
	}
}

void Renderer::print_term(std::string p_str, const Vector2I& p_pos, const Color p_fg_color, const Color p_bg_color) {
	Vector2I pos = { p_pos.x, p_pos.y / 2 };

	for (int i = 0; p_str[i]; i++) {
		if (p_str[i] == '\n') {
			pos.y++;
			pos.x = p_pos.x;
			continue;
		}

		if (pos.x < 0 || pos.x >= resolution.x ||
			pos.y < 0 || pos.y >= resolution.y / 2)
		{
			continue;
		}

		Cell& cell = m_display_buffers[m_current_buf][pos.x + resolution.x * pos.y];
		pos.x++;

		if (p_fg_color != DEFAULT) {
			cell.top = p_fg_color;
		}
		if (p_bg_color != DEFAULT) {
			cell.bottom = p_bg_color;
		}
		cell.c = p_str[i];
	}
}

void Renderer::print(std::string p_str, const Vector2I& p_pos, const Color p_fg_color, const Color p_bg_color) {
	Vector2I pos = { p_pos.x, p_pos.y};

	for (int i = 0; p_str[i]; i++) {
		short glyph = '\0';
		if (p_str[i] == '\n') {
			pos.y += m_glyph_sz.y + 2;
			pos.x = p_pos.x;
			continue;
		}


		char c = std::toupper(p_str[i]);
		if (c >= 32 && c <= 96) {
			glyph = C_GLYPHS[c - 32];
		}
		else if (c >= 123 && c <= 126) {
			glyph = C_GLYPHS[c - 58];
		}

		// Draw BG first.
		for (int y = 0; y < m_glyph_sz.y + 2; y++) {
			for (int x = 0; x < m_glyph_sz.x + 2; x++) {
				if (p_bg_color != DEFAULT) {
					draw_pixel(Vector2I(x, y) + pos, p_bg_color);
				}
			}
		}

		// Render glyph.
		for (int y = 0; y < m_glyph_sz.y; y++) {
			for (int x = 0; x < m_glyph_sz.x; x++) {
				if (glyph & (0b100000000000000 >> (m_glyph_sz.x * y + x))) {
					if (p_fg_color != DEFAULT) {
						draw_pixel(Vector2I(x, y) + pos + Vector2(1), p_fg_color);
					}
				}
			}
		}
		pos.x += m_glyph_sz.x + 1;
	}
}

void Renderer::render_screen() {
	std::string render;
	auto& buffer = m_display_buffers[m_current_buf];
	m_current_buf ^= 1; // Swap buffers.

	render += "\033[H"; // Move cursor to home position.
	for (int y = 0; y < resolution.y/2; y++) {
		for (int x = 0; x < resolution.x; x++) {
			render += fg_col_table[buffer[x + resolution.x * y].top];
			render += bg_col_table[buffer[x + resolution.x * y].bottom];
			render += buffer[x + resolution.x * y].c;
		}
		render += '\n';
	}
	render += TEXT_COL_DEFAULT;
	render += COLOR_DEFAULT;

#ifdef _WIN32
	// Fast printing for Windows.
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), render.c_str(), render.size(), nullptr, nullptr);
#else
	write(STDOUT_FILENO, render.c_str(), render.size() * sizeof(char));
#endif // _WIN32
}
