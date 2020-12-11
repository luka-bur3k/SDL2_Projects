#pragma once


//C++ headers //
#include <utility>

//SDL headers //
#include <SDL.h>

//My headers //
#include <LDZ_Texure.h>




class LDZ_Loptica {

private:
	int m_c_in_row;
	LDZ_Texture m_Texture;
	std::pair<int, int> m_velocity;
	void reset_pos();

	friend class LDZ_CollisionManager;
public:

	LDZ_Loptica(SDL_Renderer* p_red, const char* p_file)
		: m_Texture(p_red, p_file)
	{
		m_Texture.set_dst(450, 232, 9, 9);
		m_c_in_row = 0;
		m_velocity.first = 0;
		m_velocity.second = 4;
	}


	void update() {
		//std::cout << m_velocity.first << " " << m_velocity.second << std::endl;
		//std::cout << m_Texture.get_dst()->x << " " << m_Texture.get_dst()->y << std::endl;
		m_Texture.get_dst()->x += m_velocity.first;
		m_Texture.get_dst()->y += m_velocity.second;
	}


	operator LDZ_Texture() { return m_Texture; }
};