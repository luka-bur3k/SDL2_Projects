#pragma once

//C++ headers//
#include <utility>

//SDL headers//
#include <SDL.h>

//My headers//
#include <LDZ_Texure.h>


class LDZ_Palica {

private:

	LDZ_Texture m_Texture;
	void reset_pos();
	// <-- 120 - 780 --> 
	// /\  0      \/
	friend class LDZ_CollisionManager;
public:

	LDZ_Palica(SDL_Renderer* p_rend , const char * p_file)
		: m_Texture(p_rend , p_file)
	{
		m_Texture.set_dst(390, 372, 20, 120);
	}

	void setPlayerTexture(SDL_Renderer * p_renderer, const char* p_file) {
		m_Texture.set_Texture(p_renderer, p_file);
	}

	~LDZ_Palica(){}

	void setDst(int move);

	operator LDZ_Texture () const { return m_Texture; }
};