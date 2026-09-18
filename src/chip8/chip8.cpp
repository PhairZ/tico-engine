#include "chip8.h"
#include "data.h"
#include "input.h"
#include "renderer.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

void Chip8::load_rom(const std::string &filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		const std::streampos BUFF_SIZE = file.tellg();
		// Memory 0x0 to 0x199 was reserved for the interpreter.
		static constexpr uint16_t START_ADDR = 0x200u;

		file.seekg(0, std::ios::beg);
		file.read((char *)&index, 2);

		file.seekg(0, std::ios::beg);
		file.read((char *)memory + START_ADDR, BUFF_SIZE);
		file.close();

		rom_loaded = true;
	}
}

void Chip8::load_font() {
	constexpr unsigned int FONTSET_SIZE = 80u;

	uint8_t fontset[FONTSET_SIZE] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	// Store the font in 0x0 (interpreter reserved memory).
	memcpy(memory, fontset, FONTSET_SIZE);
}

void Chip8::OP_0nnn() {
	op_code = op_code & 0xFFFu;
}

void Chip8::OP_00E0() {
	memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE() {
	pc = stack[sp--];
}

void Chip8::OP_1nnn() {
	pc = op_code & 0xFFFu;
}

void Chip8::OP_2nnn() {
	sp++;
	stack[sp] = pc;
	pc = op_code & 0xFFFu;
}

void Chip8::OP_3xkk() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t kk = op_code & 0xFF;
	if (Vx == kk) {
		pc += 2;
	}
}

void Chip8::OP_4xkk() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t kk = op_code & 0xFF;
	if (Vx != kk) {
		pc += 2;
	}
}

void Chip8::OP_5xy0() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	if (Vx == Vy) {
		pc += 2;
	}
}

void Chip8::OP_6xkk() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	Vx = op_code & 0xFF;
}

void Chip8::OP_7xkk() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	Vx += op_code & 0xFF;
}

void Chip8::OP_8xy0() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	Vx = Vy;
}

void Chip8::OP_8xy1() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	Vx |= Vy;
}

void Chip8::OP_8xy2() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	Vx &= Vy;
}

void Chip8::OP_8xy3() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	Vx ^= Vy;
}

void Chip8::OP_8xy4() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	const uint16_t sum = Vx + Vy;
	Vx = sum;
	// VF register stores carry bit.
	v[0xF] = sum >> 8;
}

void Chip8::OP_8xy5() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	const uint16_t dif = Vx - Vy;
	bool flag = Vx >= Vy;
	Vx = dif;
	v[0xF] = flag;
}

void Chip8::OP_8xy6() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	bool flag = Vx & 1;
	Vx >>= 1;
	v[0xF] = flag;
}

void Chip8::OP_8xy7() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	const uint16_t dif = Vy - Vx;
	bool flag = Vy >= Vx;
	Vx = dif;
	v[0xF] = flag;
}

void Chip8::OP_8xyE() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	bool flag = Vx & 0x80;
	Vx <<= 1;
	v[0xF] = flag;
}

void Chip8::OP_9xy0() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	if (Vx != Vy) {
		pc += 2;
	}
}

void Chip8::OP_Annn() {
	index = op_code & 0xFFF;
}

void Chip8::OP_Bnnn() {
	pc = v[0] + op_code & 0xFFF;
}

void Chip8::OP_Cxkk() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	Vx = rand() & op_code & 0xFF;
}

void Chip8::OP_Dxyn() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	uint8_t &Vy = v[(op_code & 0x0F0) >> 4u];
	uint8_t height = op_code & 0x00F;

	// Tracks whether the pixel was reset. 1 for reset 0 for set
	v[0xF] = false;

	for (int y = 0; y < height; y++) {
		uint8_t sprite_byte = memory[index + y];

		for (int x = 0; x < 8; x++) {
			uint8_t sprite_pixel = sprite_byte & (0x80u >> x);

			uint8_t x_bit = (Vx + x) % VIDEO_WIDTH;
			uint8_t y_bit = (Vy + y) % VIDEO_HEIGHT;

			int bit_index = y_bit * VIDEO_WIDTH + x_bit;

			uint8_t &video_byte = video[bit_index / 8];
			uint8_t video_pixel = video_byte & (0x80u >> x_bit % 8);

			if (sprite_pixel != 0) {
				if (video_pixel != 0) {
					v[0xF] = true;
				}
				video_byte |= (0x80u >> x_bit % 8);
			}
		}
	}
}

void Chip8::OP_Ex9E() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	if (input & (1 << Vx)) {
		pc += 2;
	}
}

void Chip8::OP_ExA1() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	if (~input & (1 << Vx)) {
		pc += 2;
	}
}

void Chip8::OP_Fx07() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	Vx = dt;
}

void Chip8::OP_Fx0A() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];

	for (int i = 0; i < 16; i++) {
		if (input & (1 << i)) {
			Vx = i;
			return;
		}
	}
	pc -= 2;

	// Store pressed button in Vx.
	// Vx = input ^ old_input;
}

void Chip8::OP_Fx15() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	dt = Vx;
}

void Chip8::OP_Fx18() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	st = Vx;
}

void Chip8::OP_Fx1E() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	index += Vx;
}

void Chip8::OP_Fx29() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	index += Vx * 5;
}

void Chip8::OP_Fx33() {
	uint8_t &Vx = v[(op_code & 0xF00) >> 8u];
	memory[index + 2] = Vx % 10;
	memory[index + 1] = Vx / 10 % 10;
	memory[index] = Vx / 100 % 10;
}

void Chip8::OP_Fx55() {
	uint8_t x = (op_code & 0xF00) >> 8u;
	mempcpy(memory + index, v, x + 1);
}

void Chip8::OP_Fx65() {
	uint8_t x = (op_code & 0xF00) >> 8u;
	mempcpy(v, memory + index, x + 1);
}

void Chip8::_init() {
	m_fps_cap = 0.0f;
	load_font();
	memset(video, 0, sizeof(video));
	input = 0;

	pc = START_ADDR;

	table[0x0] = &Chip8::table_0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::table_8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::table_E;
	table[0xF] = &Chip8::table_F;

	for (int i = 0; i <= 0xE; i++) {
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	for (int i = 0; i <= 0x65; i++) {
		tableF[i] = &Chip8::OP_NULL;
	}

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;
}

void Chip8::_input_event(InputEvent *const p_event) {
	if (InputEventKey *iek = dynamic_cast<InputEventKey *>(p_event)) {
		switch (iek->key) {
		case '1': {
			input ^= 1 << 0x1;
		} break;
		case '2': {
			input ^= 1 << 0x2;
		} break;
		case '3': {
			input ^= 1 << 0x3;
		} break;
		case '4': {
			input ^= 1 << 0xC;
		} break;
		case 'q': {
			input ^= 1 << 0x4;
		} break;
		case 'w': {
			input ^= 1 << 0x5;
		} break;
		case 'e': {
			input ^= 1 << 0x6;
		} break;
		case 'r': {
			input ^= 1 << 0xD;
		} break;
		case 'a': {
			input ^= 1 << 0x7;
		} break;
		case 's': {
			input ^= 1 << 0x8;
		} break;
		case 'd': {
			input ^= 1 << 0x9;
		} break;
		case 'f': {
			input ^= 1 << 0xE;
		} break;
		case 'z': {
			input ^= 1 << 0xA;
		} break;
		case 'x': {
			input ^= 1 << 0x0;
		} break;
		case 'c': {
			input ^= 1 << 0xB;
		} break;
		case 'v': {
			input ^= 1 << 0xF;
		} break;
		}
	}
}

void Chip8::_update(double _delta) {
	op_code = memory[pc++] << 8u;
	op_code |= memory[pc++];

	(*this.*table[(op_code & 0xF000u) >> 12u])();

	if (dt) {
		dt--;
	}

	if (st) {
		st--;
	}

	speed = _delta;
}

void Chip8::_draw() {
	for (int y = 0; y < VIDEO_HEIGHT; y++) {
		for (int x = 0; x < VIDEO_WIDTH; x++) {
			// check if bit[x,y] is set.
			const unsigned int bit_idx = y * VIDEO_WIDTH + x;
			const uint8_t byte = video[bit_idx / 8];
			const bool pixel_set = byte & (0x80u >> (bit_idx % 8));
			m_renderer.draw_pixel({x, y}, (pixel_set ? Renderer::RED : Renderer::WHITE));
		}
	}

	// for (int i = 0; i < 0xF; i++) {
	// 	m_renderer.print_term(
	// 		"V" + std::to_string(i) + ":" + std::to_string(v[i]),
	// 		Vector2I(0, 36 + 2 * i),
	// 		Renderer::GREEN);
	// }

	m_renderer.print_term("      ", Vector2I(0, 100));
	m_renderer.print_term("      ", Vector2I(0, 102));
	m_renderer.print_term(input, Vector2I(0, 100));
	m_renderer.print_term(speed, Vector2I(0, 100));
}

void Chip8::_handle_input(uint16_t p_key) {}
