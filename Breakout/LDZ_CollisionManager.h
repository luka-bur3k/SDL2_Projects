#pragma once

//C++ headers
#include <utility>
#include <vector>

//SDL headers
#include <SDL.h>

//My headers
#include <LDZ_Level.h>
#include <LDZ_Palica.h>
#include <LDZ_Loptica.h>

class LDZ_CollisionManager {

private:

	LDZ_Level& m_level;
	LDZ_Palica& m_palica;
	SDL_Renderer* m_renderer;
	std::vector<LDZ_Loptica>*m_loptica;

	std::vector<std::pair<int, int>> Hit;

	void doDamage();

public:
	LDZ_CollisionManager(LDZ_Level& p_level, LDZ_Palica& p_palica, std::vector<LDZ_Loptica>* p_loptica, SDL_Renderer* p_renderer = nullptr)
		: m_level(p_level) , m_palica(p_palica) , m_loptica(p_loptica)
	{
		m_renderer = p_renderer;
		if (p_renderer != nullptr)
			m_palica.setPlayerTexture(p_renderer, m_level.m_PlayerTexture.c_str());

		m_level.m_mixer.PlayMusic(0);
		m_palica.reset_pos();
		(*m_loptica).erase(m_loptica->begin() + 1, m_loptica->end());
		(*m_loptica)[0].reset_pos();
		Hit.reserve(200);
	}

	~LDZ_CollisionManager() {
		m_level.m_mixer.PauseMusic();
	}

	bool detect(bool L , bool R);

};