#pragma once

// C++ headers //
#include <cstring>
#include <string>
#include <iostream>


// SDL headers //
#include <SDL.h>


class LDZ_Window {

private:
	SDL_Window *m_window;
	
public:
	LDZ_Window(const char* n_name, int x, int y, int w, int h, Uint32 flags) {
		m_window = SDL_CreateWindow(n_name, x, y, w, h, flags);

		if (m_window == NULL) {
			std::cout << "Window creation error" << std::endl;
		}
	}

	~LDZ_Window() {
		SDL_DestroyWindow(m_window);
	}


	operator SDL_Window*() const { return m_window; };
};
