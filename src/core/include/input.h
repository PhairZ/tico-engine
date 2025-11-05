#pragma once

#include "utils/singleton.h"

#include <vector>
#include <thread>
#ifndef _WIN32
#include <termios.h>
#endif // _WIN32

class InputEvent {
public:
	virtual ~InputEvent() = default;
	bool pressed = false;
};

class InputEventKey : public InputEvent {
public:
	char key = '\0';
};

class InputEventListener {
public:
	InputEventListener();
	virtual ~InputEventListener() = default;
	virtual void _input_event(InputEvent* const p_event) = 0;
};

class Input {
	SINGLETON(Input);
public:
	~Input();

	void add_listener(InputEventListener* p_listener);

private:
	char getch();
	void handle_input();

#ifndef _WIN32
	termios m_term_attr;
#endif // _WIN32

	bool m_reading = false;
	std::thread m_input_thread;
	std::vector<InputEventListener*> m_listeners;
};
