#pragma once

#include <LDZ_Text.h>


void LDZ_Text::Type(int x, int y, std::string p_input) {

	// 0 -> 25 slova abecede 
	// 26 -> 35 znamenke 

	std::vector<int> Natpis;

	for (int i = 0; i < p_input.size(); i++)
		if ((p_input[i] < 48 || (p_input[i] > 57 && p_input[i] < 65) || p_input[i] > 90) && p_input[i]!=32)
			return;

	for (int i = 0; i < p_input.size(); i++) {

		if (p_input[i] - '0' >= 0 && p_input[i] - '0' <= 9)
			Natpis.push_back(26 + p_input[i] - '0');
		else if (p_input[i] == 32)
			Natpis.push_back(-1);
		else	
			Natpis.push_back(p_input[i] - 'A');

	}

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = dst.h = 20;

	for (int i = 0; i < Natpis.size(); i++) {
		
		if(Natpis[i] != -1)
			SDL_RenderCopy(m_rend, m_font, &m_src[Natpis[i]], &dst);
		dst.x += 20;
	}
	

}