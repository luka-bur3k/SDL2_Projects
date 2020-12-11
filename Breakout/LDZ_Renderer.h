#pragma once

// SDL headers //
#include <SDL.h>

// My headers //
#include <LDZ_Texure.h>
#include <LDZ_Level.h>

class LDZ_Renderer {

private:
	SDL_Renderer* m_renderer;

public:
	LDZ_Renderer(SDL_Window* n_window , int index,  Uint32 flags) {
		m_renderer = SDL_CreateRenderer(n_window, index, flags);

	}
	~LDZ_Renderer() {
		SDL_DestroyRenderer(m_renderer);
	}

	void clear();
	void copy(LDZ_Texture n_tex);
	void copy(LDZ_Level& p_level);
	void present();

	operator SDL_Renderer* () const { return m_renderer; };
};