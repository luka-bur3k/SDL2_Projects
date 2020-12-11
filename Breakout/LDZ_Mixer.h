#pragma once

//C++ headers
#include <iostream>
#include <vector>

// SDL headers
#include<SDL.h>
#include<SDL_mixer.h>

class LDZ_Mixer {


private:

	std::vector<Mix_Music*> m_sample_long;
	std::vector<Mix_Chunk*> m_sample_short;

public:

	LDZ_Mixer() {
		m_sample_long.reserve(10);
		m_sample_short.reserve(10);
	}

	int AddToMix(const char* p_file , bool p_long = false) {

		if (p_file == nullptr)
			return -1;

		std::string temp = p_file;
		temp = "./" + temp;

		if (p_long == false) {
			m_sample_short.emplace_back(Mix_LoadWAV(temp.c_str()));
			if (!m_sample_short[m_sample_short.size()-1]) {
				std::cout << "Erorr loading Wav file " << temp.c_str() << std::endl;
			}
			return (int)m_sample_short.size() - 1;
		}
		else {
			m_sample_long.emplace_back(Mix_LoadMUS(temp.c_str()));
			if (!m_sample_long[m_sample_long.size() - 1]) {
				std::cout << "Erorr loading Mp3 file " << temp.c_str() << std::endl;
			}
			return (int)m_sample_long.size() - 1;
		}

	}
	~LDZ_Mixer() {
		PauseMusic();
		for (int i = 0; i < m_sample_long.size(); i++) {
			Mix_FreeMusic(m_sample_long[i]);
		}
		for (int i = 0; i < m_sample_short.size(); i++) {
			Mix_FreeChunk(m_sample_short[i]);
		}
	}

	void PlayEffect(int p_index);
	void PlayMusic(int p_index);
	void PauseMusic();

};