#pragma once

// C++ headers // 
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

//SDL headers //
#include <SDL.h>

// My headers //
#include <LDZ_Bricks.h>
#include <LDZ_Texure.h>
#include <LDZ_Mixer.h>

// TinyXML parser //
#include <tinyxml2.h>

extern unsigned int Lifes;

class LDZ_Level {

private:
	int m_RowCount, m_ColumnCount, m_RowSpacing, m_ColumnSpacing;
	unsigned int m_totalBlocks;
	std::string m_BackgroundTexture;
	std::string m_PlayerTexture;
	std::vector<LDZ_Bricks> m_BrickTypes;
	std::vector<std::vector<LDZ_Bricks>> m_LevelLayout;
	std::vector<std::vector<LDZ_Texture>> m_TextureLayout;
	LDZ_Mixer m_mixer;


	void reset_Level();


	friend class LDZ_CollisionManager;

public:

	LDZ_Level(const char* p_filepath);
	~LDZ_Level(){
		for (int i = 0; i < m_TextureLayout.size(); i++)
			for (int j = 0; j < m_TextureLayout[i].size(); j++)
				m_TextureLayout[i][j].Destroy();
		m_Background.Destroy();
	}

	void LoadTextures(SDL_Renderer * p_renderer);
	auto& getLevelLayout() {
		return m_LevelLayout;
	}
	auto& getTextureLayout() {
		return m_TextureLayout;
	}

	const char* getBackGround() {
		return m_BackgroundTexture.c_str();
	}

	void DEBUG_PRINT() {
		for (int i = 0; i < m_LevelLayout.size(); i++)
			for (int j = 0; j < m_LevelLayout[i].size(); j++)
				std::cout << m_LevelLayout[i][j].getTexture() << " <---> ";
	}

	LDZ_Texture m_Background;
};