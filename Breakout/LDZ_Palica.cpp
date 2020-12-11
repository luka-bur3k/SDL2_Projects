#include <LDZ_Palica.h>

void LDZ_Palica::reset_pos() {
	m_Texture.set_dst(390, 372, 20, 120);
	return;
}

void LDZ_Palica::setDst(int move) {
	// pomak ulijevo je -x , udesno je +x
	if (m_Texture.get_dst()->x + move < 120)
		m_Texture.get_dst()->x = 120;
	else if (m_Texture.get_dst()->x  + move > 660)
		m_Texture.get_dst()->x = 660;
	else
		m_Texture.get_dst()->x += move;
}