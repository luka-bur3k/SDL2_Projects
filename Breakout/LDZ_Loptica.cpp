#include <LDZ_Loptica.h>

void LDZ_Loptica::reset_pos() {
	m_c_in_row = 0;
	m_Texture.set_dst(450, 232, 9, 9);
	m_velocity.first = 0;
	m_velocity.second = 4;
}