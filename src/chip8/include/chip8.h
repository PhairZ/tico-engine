#pragma once

#include "data.h"
#include "game.h"
#include <cstdint>

class Chip8 : public Game {
public:
	void load_rom(const std::string &filename);
	bool is_rom_loaded() const { return rom_loaded; }

private:
	void _init() override;
	void _input_event(InputEvent *const p_event) override;
	void _update(double p_delta) override;
	void _draw() override;

	void _handle_input(uint16_t p_key);

	bool rom_loaded = false;

	// Memory:
	static constexpr uint16_t MEMORY_SIZE_BYTES = 4096u;
	// Memory 0x0 to 0x199 was reserved for the interpreter.
	static constexpr uint16_t START_ADDR = 0x200u;
	static constexpr uint8_t STACK_LEVELS = 16u;
	static constexpr uint8_t VIDEO_WIDTH = 64u;
	static constexpr uint8_t VIDEO_HEIGHT = 32u;
	uint8_t memory[MEMORY_SIZE_BYTES];
	// Video memory. Each pixel is a bit either on or of.
	uint8_t video[VIDEO_WIDTH * VIDEO_HEIGHT / sizeof(uint8_t)];
	// Holds a pointer to the instruction in memory before allocating new stack.
	uint16_t stack[STACK_LEVELS];

	double speed = 0;
	// Registers:
	uint16_t index;
	// Program Counter.
	// Points to the line to execute, and updated right after instruction fetch.
	uint16_t pc;
	// Stack Pointer.
	// This is an index to the current stack level used.
	// Chip-8 has a limit of 16 stack levels.
	uint8_t sp;
	uint8_t v[16];
	// If the DT register is non-zero, it decrements every 60Hz.
	uint8_t dt = 0u;
	// If the ST register is non-zero, it decrements every 60Hz.
	// I won't bother with implementing sounds for now.
	uint8_t st = 0u;
	// A bitmask of 16 keys supported by Chip-8.
	uint16_t input;

	void load_font();
	void flip_pixel(Vector2I p_pos);

	// Instructions:
	uint16_t op_code;

	void OP_NULL() {} // for invalid instructions.
	void OP_0nnn();	  // SYS addr
	void OP_00E0();	  // CLS
	void OP_00EE();	  // RET
	void OP_1nnn();	  // JP addr
	void OP_2nnn();	  // CALL addr
	void OP_3xkk();	  // SE Vx, byte
	void OP_4xkk();	  // SNE Vx, byte
	void OP_5xy0();	  // SE Vx, Vy
	void OP_6xkk();	  // LD Vx, byte
	void OP_7xkk();	  // ADD Vx, byte
	void OP_8xy0();	  // LD Vx, Vy
	void OP_8xy1();	  // OR Vx, Vy
	void OP_8xy2();	  // AND Vx, Vy
	void OP_8xy3();	  // XOR Vx, Vy
	void OP_8xy4();	  // ADD Vx, Vy
	void OP_8xy5();	  // SUB Vx, Vy
	void OP_8xy6();	  // SHR Vx {, Vy}
	void OP_8xy7();	  // SUBN Vx, Vy
	void OP_8xyE();	  // SHL Vx {, Vy}
	void OP_9xy0();	  // SNE Vx, Vy
	void OP_Annn();	  // LD I, addr
	void OP_Bnnn();	  // JP V0, addr
	void OP_Cxkk();	  // RND Vx, byte
	void OP_Dxyn();	  // DRW Vx, Vy, nibble
	void OP_Ex9E();	  // SKP Vx
	void OP_ExA1();	  // SKNP Vx
	void OP_Fx07();	  // LD Vx, DT
	void OP_Fx0A();	  // LD Vx, K
	void OP_Fx15();	  // LD DT, Vx
	void OP_Fx18();	  // LD ST, Vx
	void OP_Fx1E();	  // ADD I, Vx
	void OP_Fx29();	  // LD F, Vx
	void OP_Fx33();	  // LD B, Vx
	void OP_Fx55();	  // LD [I], Vx
	void OP_Fx65();	  // LD Vx, [I]

	// Instruction tables:
	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];

	void table_0() { (*this.*table0[op_code & 0x000F])(); }

	void table_8() { (*this.*table8[op_code & 0x000F])(); }

	void table_E() { (*this.*tableE[op_code & 0x000F])(); }

	void table_F() { (*this.*tableF[op_code & 0x00FF])(); }
};
