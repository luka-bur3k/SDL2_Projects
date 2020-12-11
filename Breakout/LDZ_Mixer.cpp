
#include <LDZ_Mixer.h>

void LDZ_Mixer::PlayEffect(int p_index) {

	Mix_PlayChannel(-1, m_sample_short[p_index], 0);

}

void LDZ_Mixer::PlayMusic(int p_index) {

	Mix_PlayMusic(m_sample_long[p_index], -1);
}

void LDZ_Mixer::PauseMusic() {

	if (Mix_PausedMusic())
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();

}