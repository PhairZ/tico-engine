#include "input.h"

#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif // _WIN32

Input::Input() {
#ifndef _WIN32
    if (tcgetattr(STDIN_FILENO, &m_term_attr) < 0) {
        perror("tcgetattr()");
    }
    // Don't wait for enter, nor print the pressed key.
    termios new_attr = m_term_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attr) < 0) {
        perror("tcsetattr ICANON");
    }

#endif // _WIN32
    m_reading = true;
    if (!m_input_thread.joinable()) {
        m_input_thread = std::thread(&Input::handle_input, this);
    }
}

Input::~Input() {
    m_reading = false;
    if (m_input_thread.joinable()) {
        m_input_thread.join();
    }
#ifndef _WIN32

    if (tcsetattr(STDIN_FILENO, TCSANOW, &m_term_attr) < 0) {
        perror("tcsetattr ICANON");
    }
#endif // _WIN32
}

char Input::getch() {
#ifdef _WIN32
    return _getch();
#else
    char buf = 0;
    if (read(STDIN_FILENO, &buf, 1) < 0)
        perror("read()");
    return buf;
#endif
}

void Input::add_listener(InputEventListener* p_listener) {
	m_listeners.push_back(p_listener);
}

void Input::handle_input() {
	while (m_reading) {
		InputEventKey iek;
		iek.key = getch();
		iek.pressed = true;

        // Maybe reading has been already set to false while waiting for getch().
        if (!m_reading) {
            break;
        }

		for (InputEventListener* listener : m_listeners) {
			listener->_input_event(&iek);
		}
	}
}

InputEventListener::InputEventListener() {
    Input::get_singleton().add_listener(this);
}