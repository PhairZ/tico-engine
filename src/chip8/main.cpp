#include "chip8.h"
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " \"path/to/rom\"" << std::endl;
		return 0;
	}

	Chip8 game;
	game.load_rom(argv[1]);
	if (game.is_rom_loaded()) {
		game.run();
	} else {
		std::cout << "Couldn't find rom at path \"" << argv[1] << "\"" << std::endl;
		return 0;
	}
}
