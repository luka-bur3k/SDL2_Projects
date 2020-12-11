#pragma once

//C++ headers
#include<iostream>

// SDL headers //
#include <SDL.h>
#include <SDL_image.h>

// GLI headers //
//#include <gli.hpp>



class LDZ_Texture {

private:
	
	SDL_Texture* m_texture;
	SDL_Rect* m_src, *m_dst;

	friend class LDZ_CollisionManager;
public:
	LDZ_Texture() {
		m_texture = nullptr;
		m_src = m_dst = nullptr;
	}
	LDZ_Texture(SDL_Renderer* n_renderer , const char * n_file) {
			
		
		if (n_file != nullptr) {
			m_texture = IMG_LoadTexture(n_renderer, n_file);
		}
		else
			m_texture = nullptr;
		m_dst = m_src = NULL;
	}
	void set_Texture(SDL_Renderer* n_renderer, const char* n_file) {
		this->Destroy();
		if (n_file != nullptr)
			m_texture = IMG_LoadTexture(n_renderer, n_file);
		else
			m_texture = nullptr;
		m_dst = m_src = NULL;	
	}
	~LDZ_Texture() {
	}

	void Destroy() {
		if(m_dst != nullptr) delete(m_dst);
		m_dst = nullptr;
		if(m_texture != nullptr) SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}

	inline SDL_Rect* get_src() { return m_src; };
	inline SDL_Rect* get_dst() { return m_dst; };
	
	void set_dst(int x, int y, int h, int w) {

		if (x == y && y == h && h == w && w == -1) {
			if(m_dst != nullptr) delete(m_dst);
			m_dst = nullptr;
			return;
		}
		else if (m_dst == nullptr) {
			m_dst = new SDL_Rect();
		}
		m_dst->x = x;
		m_dst->y = y;
		m_dst->w = w;
		m_dst->h = h;

	}

	operator SDL_Texture* () const { return m_texture; }
};