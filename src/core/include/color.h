#pragma once

#include "renderer.h"

#define COLOR_BLACK "\033[40m" // 0 0 0
#define COLOR_RED "\033[41m" // 1 0 0
#define COLOR_GREEN "\033[42m" // 0 1 0
#define COLOR_YELLOW "\033[43m" // 1 1 0
#define COLOR_BLUE "\033[44m" // 0 0 1
#define COLOR_MAGENTA "\033[45m" // 1 0 1
#define COLOR_CYAN "\033[46m" // 0 1 1
#define COLOR_WHITE "\033[47m" // 1 1 1
#define COLOR_DEFAULT "\033[49m"

#define TEXT_COL_BLACK "\033[30m"
#define TEXT_COL_RED "\033[31m"
#define TEXT_COL_GREEN "\033[32m"
#define TEXT_COL_YELLOW "\033[33m"
#define TEXT_COL_BLUE "\033[34m"
#define TEXT_COL_MAGENTA "\033[35m"
#define TEXT_COL_CYAN "\033[36m"
#define TEXT_COL_WHITE "\033[37m"
#define TEXT_COL_DEFAULT "\033[39m"

static constexpr const char* bg_col_table[Renderer::DEFAULT + 1] = {
	COLOR_BLACK,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_WHITE,
	COLOR_DEFAULT
};

static constexpr const char* fg_col_table[Renderer::DEFAULT + 1] = {
	TEXT_COL_BLACK,
	TEXT_COL_RED,
	TEXT_COL_GREEN,
	TEXT_COL_YELLOW,
	TEXT_COL_BLUE,
	TEXT_COL_MAGENTA,
	TEXT_COL_CYAN,
	TEXT_COL_WHITE,
	TEXT_COL_DEFAULT
};