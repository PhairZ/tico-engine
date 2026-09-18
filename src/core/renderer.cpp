#include "renderer.h"
#include "color.h"
#include "data.h"
#include "font.h"

#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#define null_device "NUL"
#else
#include <unistd.h>
#endif // _WIN32

Renderer::Renderer() {
	FILE *nullFile = fopen("/dev/null", "w");
	if (nullFile) {
		fflush(stderr);
		dup2(fileno(nullFile), fileno(stderr));
		fclose(nullFile);
	}

	std::cout << "\033[?25l"; // Hide Cursor.
	std::iostream::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
}

Renderer::~Renderer() {
	std::cout << "\033[?25h"; // Show Cursor.
}

void Renderer::draw_pixel(const Vector2I &p_pos, const Color p_color) {
	if (p_pos.x < 0 || p_pos.x >= RESOLUTION.x || p_pos.y < 0 || p_pos.y >= RESOLUTION.y) {
		return;
	}

	auto &current_buf = m_display_buffer;
	current_buf[p_pos.x + RESOLUTION.x * (p_pos.y / 2)].c = "▀";
	if (p_pos.y & 1) {
		current_buf[p_pos.x + RESOLUTION.x * (p_pos.y / 2)].bottom = p_color;
	} else {
		current_buf[p_pos.x + RESOLUTION.x * (p_pos.y / 2)].top = p_color;
	}
}

void Renderer::draw_rect(
	const Vector2I &p_position, const Vector2I &p_size, Renderer::Color p_color) {
	for (int y = 0; y < p_size.y; y++) {
		for (int x = 0; x < p_size.x; x++) {
			draw_pixel(p_position + Vector2I(x, y), p_color);
		}
	}
}

void Renderer::draw_sprite(
	const char *p_sprite, const Vector2I &p_position, bool flip_h, bool flip_v,
	const Vector2I &p_size) {
	for (int y = 0; y < p_size.y; y++) {
		for (int x = 0; x < p_size.x; x++) {
			int index = y * p_size.x + x;
			Renderer::Color color = static_cast<Renderer::Color>(p_sprite[index]);
			if (color == Renderer::TRANSPARENT) {
				continue;
			}
			Vector2I pos =
				p_position + Vector2I(flip_h ? p_size.x - x - 1 : x, flip_v ? p_size.y - y - 1 : y);
			draw_pixel(pos, color);
		}
	}
}

void Renderer::clear(Color p_color) {
	for (int y = 0; y < RESOLUTION.y; y++) {
		for (int x = 0; x < RESOLUTION.x; x++) {
			draw_pixel({x, y}, p_color);
		}
	}
}

void Renderer::print_term(
	const Printable &p_text, const Vector2I &p_pos, const Color p_fg_color,
	const Color p_bg_color) {
	Vector2I pos = {p_pos.x, p_pos.y / 2};

	for (int i = 0; p_text[i]; i++) {
		if (p_text[i] == '\n') {
			pos.y++;
			pos.x = p_pos.x;
			continue;
		}

		if (pos.x < 0 || pos.x >= RESOLUTION.x || pos.y < 0 || pos.y >= RESOLUTION.y / 2) {
			continue;
		}

		Cell &cell = m_display_buffer[pos.x + RESOLUTION.x * pos.y];
		pos.x++;

		if (p_fg_color != TRANSPARENT) {
			cell.top = p_fg_color;
		}
		if (p_bg_color != TRANSPARENT) {
			cell.bottom = p_bg_color;
		}
		cell.c = p_text[i];
	}
}

void Renderer::print(
	const Printable &p_text, const Vector2I &p_pos, const Color p_fg_color,
	const Color p_bg_color) {
	Vector2I pos = {p_pos.x, p_pos.y};

	for (int i = 0; p_text[i]; i++) {
		if (p_text[i] == '\n') {
			pos.y += m_glyph_sz.y + 2;
			pos.x = p_pos.x;
			continue;
		}

		char c = std::toupper(p_text[i]);
		short glyph = '\0';
		if (c >= 32 && c <= 96) {
			glyph = C_GLYPHS[c - 32];
		} else if (c >= 123 && c <= 126) {
			glyph = C_GLYPHS[c - 58];
		}

		for (int y = -1; y < m_glyph_sz.y + 1; y++) {
			for (int x = -1; x < m_glyph_sz.x + 1; x++) {
				if (x >= 0 && y >= 0 && x < m_glyph_sz.x && y < m_glyph_sz.y &&
					p_fg_color != TRANSPARENT &&
					glyph & (0b100000000000000 >> (m_glyph_sz.x * y + x))) {
					draw_pixel(Vector2I(x, y) + pos + Vector2I(1), p_fg_color);
					continue;
				}
				if (p_bg_color != TRANSPARENT) {
					draw_pixel(Vector2I(x, y) + pos + Vector2I(1), p_bg_color);
				}
			}
		}

		pos.x += m_glyph_sz.x + 1;
	}
}

void Renderer::render_screen() {
	std::string render;

	render += "\033[H"; // Move cursor to home position.
	for (int y = 0; y < RESOLUTION.y / 2; y++) {
		for (int x = 0; x < RESOLUTION.x; x++) {
			render += fg_col_table[m_display_buffer[x + RESOLUTION.x * y].top];
			render += bg_col_table[m_display_buffer[x + RESOLUTION.x * y].bottom];
			render += m_display_buffer[x + RESOLUTION.x * y].c;
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
