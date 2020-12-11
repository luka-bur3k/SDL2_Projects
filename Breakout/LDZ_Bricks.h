#pragma once

#include <sstream>
#include <cstring>
#include <string>

extern unsigned int Score;

class LDZ_Bricks {
	// Moglo bi se rec entity class 
	
private:
		
	char m_Id;
	std::string m_Texture;
	int m_HitPoints;
	std::string m_HitSound;
	std::string m_BreakSound;
	unsigned int m_BreakScore;
	bool m_special;

	int m_HitSoundIndex;
	int m_BreakSoundIndex;

	friend class LDZ_Level;
	friend class LDZ_CollisionManager;
public:

	LDZ_Bricks(const char* p_Id , const char* p_Texture , const char* p_HitPoints ,
				const char* p_HitSound , const char* p_BreakSound , const char* p_BreakScore )
	{
		m_HitSoundIndex = m_BreakSoundIndex = -1;
		m_HitPoints = m_BreakScore = 0;


		std::stringstream ss;
		ss << p_Id;
		ss >> m_Id;
		ss.str("");
		ss.clear();

		if (m_Id >= 97 && m_Id <= 122)
			m_special = true;
		else
			m_special = false;

		m_Texture = p_Texture;
		m_HitSound = p_HitSound;


		if (p_BreakScore != nullptr) {
			ss << p_HitPoints;
			ss >> m_HitPoints;
			ss.str("");
			ss.clear();

			ss << p_BreakScore;
			ss >> m_BreakScore;

			m_BreakSound = p_BreakSound;
		}
		else {
			m_HitPoints = 9999;
			// kvazi inf 
		}
		

		//std::cout << "Brick \n\t" << m_Id << " , " << m_Texture << " " << m_HitPoints << " \n\t" << m_HitSound << " " << ((m_BreakSound == nullptr) ? "NaN" : m_BreakSound) << " " << m_BreakScore << std::endl;
	}

	LDZ_Bricks(char p_Id, const char* p_Texture, int p_HitPoints,
		const char* p_HitSound, const char* p_BreakSound, int p_BreakScore)
		: m_Id(p_Id) , m_HitPoints(p_HitPoints) ,  m_Texture(""), m_HitSound(""),
		m_BreakSound("") , m_BreakScore(p_BreakScore)
		//null brick constructor
	{
		m_special = false;
		m_HitSoundIndex = m_BreakSoundIndex = -1;
		
	}

	LDZ_Bricks(const LDZ_Bricks& p_cpy) {
		m_Id = p_cpy.m_Id;
		m_Texture = p_cpy.m_Texture;
		m_HitSound= p_cpy.m_HitSound;
		m_HitPoints= p_cpy.m_HitPoints;
		m_BreakSound = p_cpy.m_BreakSound;
		m_BreakScore= p_cpy.m_BreakScore;
		m_BreakSoundIndex = p_cpy.m_BreakSoundIndex;
		m_HitSoundIndex = p_cpy.m_HitSoundIndex;
		m_special = p_cpy.m_special;
	}

	~LDZ_Bricks() {
	}

	inline const char* getTexture() const {
		//std::cout << m_Texture << std::endl;
		return m_Texture.c_str();

	}

	inline char getId() const {
		return m_Id;
	}

	inline void setIndeces(int p_hs , int p_bs) {
		m_HitSoundIndex = p_hs;
		m_BreakSoundIndex = p_bs;
	}

	inline bool Hit() {
		m_HitPoints--;
		if (m_HitPoints == 0) {
			Score += m_BreakScore;
		}
			
		return m_HitPoints == 0;
	}
};