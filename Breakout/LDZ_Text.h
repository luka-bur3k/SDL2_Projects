#pragma once

//C++ headers
#include<string>
#include <vector>

// SDL headers 
#include <SDL.h>
#include <SDL_image.h>

// My headers 
#include <LDZ_Texure.h>

class LDZ_Text {

private:
	
	SDL_Texture* m_font;
	SDL_Renderer* m_rend;
	SDL_Rect m_src[40];

public:
	LDZ_Text(SDL_Renderer* p_rend , const char * p_fontfile)
	{
		m_rend = p_rend;
		m_font = IMG_LoadTexture(p_rend, p_fontfile);

		for(int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++) {
				m_src[i * 6 + j].x = 1 + j * 21;
				m_src[i * 6 + j].y = 1 + i * 21;
				m_src[i * 6 + j].w = m_src[i * 6 + j].h = 20;
			}
	}

	~LDZ_Text() {

	}

	void Type(int x, int y, std::string p_input);


};
