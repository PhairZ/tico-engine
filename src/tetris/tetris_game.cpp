#include "tetris_game.h"
#include "level.h"

#include <random>
#include <algorithm>

void Tetris::spawn_block(const Vector2I& p_pos) {
	curr_block = std::move(next_block);

	std::random_device rd;
	std::default_random_engine gen(rd());
	
	std::uniform_int_distribution<> dist(1, Block::C_VARIATIONS);

	switch (dist(gen)) {
	case 1:
		next_block = std::make_unique<OTetroid>(p_pos, Renderer::BLACK);
		break;
	case 2:
		next_block = std::make_unique<TTetroid>(p_pos, Renderer::GREEN);
		break;
	case 3:
		next_block = std::make_unique<LTetroid>(p_pos, Renderer::RED);
		break;
	case 4:
		next_block = std::make_unique<JTetroid>(p_pos, Renderer::BLUE);
		break;
	case 5:
		next_block = std::make_unique<STetroid>(p_pos, Renderer::MAGENTA);
		break;
	case 6:
		next_block = std::make_unique<ZTetroid>(p_pos, Renderer::YELLOW);
		break;
	case 7:
		next_block = std::make_unique<ITetroid>(p_pos, Renderer::BLUE);
		break;
	default:
		break;
	}

	if (curr_block == nullptr) {
		spawn_block();
		return;
	}
	curr_block->draw();
}

void Tetris::_init() {
	m_renderer.set_resolution({ 25, 20 });
	spawn_block();

	// Level borders.
	for (int y = 0; y <= Level::LVL_SIZE_Y; y++) {
		for (int x = 0; x <= Level::LVL_SIZE_X; x++) {
			if (x == 0 || y == Level::LVL_SIZE_Y || x == Level::LVL_SIZE_X) {
				Level::get_singleton().set_cell({ x, y }, { Renderer::CYAN });
			} else {
				Level::get_singleton().set_cell({ x, y }, { Renderer::DEFAULT });
			}
		}
	}
}

void Tetris::_input_event(InputEvent* const p_event) {
	if (auto iek = dynamic_cast<InputEventKey*>(p_event)) {
		switch (iek->key) {
		case 'w':
			while (curr_block->move({ 0, 1 }) == 0);
			break;
		case 'a':
			curr_block->move({ -1, 0 });
			break;
		case 'd':
			curr_block->move({ 1, 0 });
			break;
		case 's':
			curr_block->move({ 0, 1 });
			break;
		case 'q':
			curr_block->rotate(true);
			break;
		case 'e':
			curr_block->rotate(false);
			break;
		case 27:
			m_running = false;
			break;
		default:
			break;
		}
	}
}

void Tetris::_update(double p_delta) {
	static double ticks = 0;
	ticks += 20 * p_delta;

	// Update relative to score. Higher score results in higher tick rate.
	int score = m_singles * 1 + m_doubles * 3 + m_triples * 5 + m_quadruples * 7;
	if (ticks > std::max(1.0, 10.0 - score/10)) {
		ticks = 0;

		if (curr_block->move({ 0, 1 }) == -1) {
			// If spawn has a block in it, you lose.
			if (Level::get_singleton().get_cell(C_SPAWN_POS).color != Renderer::DEFAULT) {
				m_running = false;
				return;
			}

			// Check if there are any cleared lines.
			switch (Level::get_singleton().clear_line(curr_block->get_position().y)) {
				case 1:
					m_singles++;
					break;
				case 2:
					m_doubles++;
					break;
				case 3:
					m_triples++;
					break;
				case 4:
					m_quadruples++;
					break;
				default:
					break;
			}

			spawn_block();
		}
	}
}

void Tetris::_draw() {
	// Level
	{
		for (int y = 0; y <= Level::LVL_SIZE_Y; y++) {
			for (int x = 0; x <= Level::LVL_SIZE_X; x++) {
				auto cell = Level::get_singleton().get_cell({
					x,
					y
				});
				m_renderer.draw_pixel({ x, y }, (cell.color == Renderer::DEFAULT ? Level::bg : cell.color));
			}
		}
	}

	// UI
	{
		constexpr int UI_START = Level::LVL_SIZE_X + 1;
		const int UI_WIDTH = 7;
		for (int y = 0; y < m_renderer.get_resolution().y; y++) {
			for (int x = UI_START; x < m_renderer.get_resolution().x; x++) {
				m_renderer.draw_pixel({ x, y }, Renderer::CYAN);
			}
		}

		m_renderer.print_term(" TERTIS ", { UI_START + 1, 2 }, Renderer::BLACK, Renderer::WHITE);
		m_renderer.print_term(" Terminal  ", { UI_START, 4 }, Renderer::BLACK, Renderer::WHITE);
		m_renderer.print_term("    Tetris ", { UI_START, 5 }, Renderer::BLACK, Renderer::WHITE);
		m_renderer.print_term(" by", { UI_START, 5 }, Renderer::BLACK, Renderer::WHITE);
		m_renderer.print_term(" PhairZ ", {UI_START + 3, 6 }, Renderer::BLUE, Renderer::WHITE);

		char buf[6];
		snprintf(buf, 6, "%5d", m_singles + m_doubles * 2 + m_triples * 3  + m_quadruples * 4);
		m_renderer.print_term("Lines:", { UI_START, 10 }, Renderer::WHITE, Renderer::CYAN);
		m_renderer.print_term(buf, { UI_START + 6, 10 }, Renderer::BLACK, Renderer::WHITE);

		snprintf(buf, 6, "%5d", m_singles * 10 + m_doubles * 30 + m_triples * 50 + m_quadruples * 70);
		m_renderer.print_term("Score:", { UI_START, 12 }, Renderer::WHITE, Renderer::CYAN);
		m_renderer.print_term(buf, { UI_START + 6, 12 }, Renderer::BLACK, Renderer::WHITE);

		// Next Block. (4x4)
		for (int y = 15; y < 19; y++) {
			for (int x = UI_START + 1; x < UI_START + 5; x++) {
				auto shape = next_block->get_shape();
				auto tile = std::find(
					shape.begin(), shape.begin() + 4,
					Vector2I(x - 15, y - 16)
				);
				if (tile == shape.begin() + 4) {
					m_renderer.draw_pixel({ x, y }, Renderer::WHITE);
				} else {
					m_renderer.draw_pixel({ x, y }, next_block->color);
				}
			}
		}
	}
}