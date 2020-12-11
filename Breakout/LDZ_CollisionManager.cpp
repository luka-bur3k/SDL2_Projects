#include <LDZ_CollisionManager.h>

extern bool LoadNextLevel;
extern bool PauseNextLife;

struct Ratio_Coll
{
	// internalni struct korišten za CollisionSystem
	Ratio_Coll(int p_t, int p_v, int p_h)
		:type_of_collision(p_t) , vertical(p_v), horizontal(p_h)
	{}
	int type_of_collision;
	int vertical;
	int horizontal;
};

static Ratio_Coll get_collision_points(SDL_Rect src , SDL_Rect dst) {

	Ratio_Coll ret(-1,0,0);

	src.h += src.y; // uvijek loptica
	src.w += src.x; // (x ,y) (w,h)

	dst.h += dst.y; // palica , blok 
	dst.w += dst.x; 

	//std::cout << src.x << " " << src.y << " " << src.w << " " << src.h << std::endl;
	//std::cout << dst.x << " " << dst.y << " " << dst.w << " " << dst.h << std::endl;
	//std::cout << "---------------------------\n";

	if (src.h < dst.y || src.y > dst.h || src.w < dst.x || src.x > dst.w)
		return ret;
	int tester[4];
	tester[1] = (src.x - dst.x );
	tester[0]= (src.y - dst.y );
	tester[3] = (src.w - dst.w );
	tester[2] = (src.h - dst.h );

	if (tester[0] <= 0 && tester[1] >= 0 && tester[2] <= 0 && tester[3] <= 0) { // FTFF DOWN 3
		ret.type_of_collision = 3;
		ret.horizontal = 15; // nebitna vrijednost
	}
	else if (tester[0] >= 0 && tester[1] <= 0 && tester[2] <= 0 && tester[3] <= 0) { // TFFF RIGHT 2 
		ret.type_of_collision = 2;
		ret.vertical = 15;
	}
	else if (tester[0] >= 0 && tester[1] >= 0 && tester[2] <= 0 && tester[3] >= 0) { // TTFT LEFT 0
		ret.type_of_collision = 0;
		ret.vertical = 15;
	}
	else if (tester[0] >= 0 && tester[1] >= 0 && tester[2] >= 0 && tester[3] <= 0) { // TTTF UP 1
		ret.type_of_collision = 1;
		ret.horizontal = 15;
	}


	else if (tester[0] <= 0 && tester[1] <= 0 && tester[2] <= 0 && tester[3] <= 0) { // FFFF RIGHT or DOWN
		ret.vertical = src.h - dst.y;
		ret.horizontal = src.w - dst.x;
		ret.type_of_collision = (ret.horizontal > ret.vertical ? 3 : 2);
	}
	else if (tester[0] <= 0 && tester[1] >= 0 && tester[2] <= 0 && tester[3] >= 0) { // FTFT DOWN or LEFT 
		ret.vertical = src.h - dst.y;
		ret.horizontal = dst.w - src.x;
		ret.type_of_collision = (ret.horizontal > ret.vertical ? 3 : 0);
	}
	else if (tester[0] >= 0 && tester[1] <= 0 && tester[2] >= 0 && tester[3] <= 0) { // TFTF RIGHT or UP
		ret.vertical = dst.h - src.y;
		ret.horizontal = src.w - dst.x;
		ret.type_of_collision = (ret.horizontal > ret.vertical ? 1 : 2);
	}
	else if (tester[0] >= 0 && tester[1] >= 0 && tester[2] >= 0 && tester[3] >= 0) { // TTTT LEFT or UP
		ret.vertical = dst.h - src.y;
		ret.horizontal = dst.w - src.x;
		ret.type_of_collision = (ret.horizontal > ret.vertical ? 1 : 0);
	}
	//else {
	//	std::cout << "Imposiburu" << std::endl;
	//	std::cout << src.x << " " << src.y << " " << src.w << " " << src.h << std::endl;
	//	std::cout << dst.x << " " << dst.y << " " << dst.w << " " << dst.h << std::endl;
	//	std::cout << "---------------------------\n";
	//}

	/*
	Uvjeti za collision detection funkcioniraju otprilike ovako 

		1	 2    3
			----
		4	| 9|  5
			----
		6    7    8 

	stanje 9 - imposiburu 
	stanje 1 pravokutnik src se kriza u cosku gore lijevo i tu je bitan omjer kolikom duzinom se kriza da znamo u kojem smjeru se odbija
	stanja 3 ,6 ,8 su identicna

	stanje 2 ,4 ,5 ,7 se samo preklapaju u samim stranicama i ne diraju vrhove tu je jednoznacna strana u koju se odbija
	*/

	return ret;
}



bool LDZ_CollisionManager::detect(bool L , bool R) {

	std::vector<int> ispali;
	bool change = false;

	for (int i = 0; i < (*m_loptica).size(); i++) {
		bool collisions[4] = { false , false , false , false };
		//					   LEFT ,   UP  ,  RIGHT ,  DOWN

		auto coord = *((*m_loptica)[i].m_Texture.get_dst());

		if (coord.y >= 403 && (*m_loptica).size() == 1) {
			//std::cout << "NE SMIJE" << std::endl;
			PauseNextLife = true;
			m_level.reset_Level();
			m_palica.reset_pos();
			(*m_loptica)[i].reset_pos();
		} 
		else if (coord.y >= 403) {
			ispali.push_back(i);
			continue;
		}


		if (coord.x <= 117)
			collisions[0] = true;
		if (coord.y <= 9)
			collisions[1] = true;
		if (coord.x >= 771)
			collisions[2] = true;
		// 3 odbijanja od samog levela 


		//---------------------------------------------------//

		Ratio_Coll TestColl(0, 0, 0);
		auto& coord2 = m_level.m_TextureLayout;

		for (int i = 0; i < coord2.size(); i++)
			for (int j = 0; j < coord2[i].size(); j++) {

				if ((SDL_Texture*)coord2[i][j] == nullptr)
					continue;

				TestColl = get_collision_points(coord, *(coord2[i][j].get_dst()));
				if (TestColl.type_of_collision != -1) {
					collisions[TestColl.type_of_collision] = true;
					Hit.push_back(std::make_pair(i, j));
	
					// koga smo sve pogodili
				}

			}


		auto& coord3 = *(m_palica.m_Texture.get_dst());
		//std::cout << coord.x << " " << coord.y << "\n";

		//---------------------------------------------------//

		TestColl = get_collision_points(coord, coord3);
		//std::cout << TestColl.type_of_collision << "|" << TestColl.horizontal << " " << TestColl.vertical << std::endl;
		if (TestColl.type_of_collision != -1) {
			// ako je pogodena palica 
			(*m_loptica)[i].m_c_in_row = 0;
			collisions[TestColl.type_of_collision] = true;
			(*m_loptica)[i].m_velocity.first += (L == true ? -1 : (R == true ? 1 : 0));
			if (abs((*m_loptica)[i].m_velocity.first) > 3)
				(*m_loptica)[i].m_velocity.first = ((*m_loptica)[i].m_velocity.first < 0 ? -3 : 3);
		}

		//---------------------------------------------------//

		// ovaj dio koda je u svrhu sprjecavanja rapidnih odbijanja lijevo desno
		// ako loptica dodirne u istom trenutku vise blokova 
		if (collisions[0] == collisions[2] && collisions[0] == true) {
			collisions[0] = collisions[2] = 0;
			if ((*m_loptica)[i].m_velocity.second < 0)
				collisions[3] = true;
			else
				collisions[1] = true;
		}
		else if (collisions[1] == collisions[3] && collisions[1] == true) {
			collisions[1] = collisions[3] = 0;
			if ((*m_loptica)[i].m_velocity.first > 0)
				collisions[0] = true;
			else
				collisions[2] = true;
		}

		//---------------------------------------------------//


		if (collisions[0] == true && (*m_loptica)[i].m_velocity.first < 0) {
			(*m_loptica)[i].m_velocity.first = -(*m_loptica)[i].m_velocity.first;
			change = true;
		}
		else if (collisions[2] == true && (*m_loptica)[i].m_velocity.first > 0) {
			(*m_loptica)[i].m_velocity.first = -(*m_loptica)[i].m_velocity.first;
			change = true;
		}

		if (collisions[1] == true && (*m_loptica)[i].m_velocity.second < 0) {
			(*m_loptica)[i].m_velocity.second = -(*m_loptica)[i].m_velocity.second;
			change = true;
		}
		else if (collisions[3] == true && (*m_loptica)[i].m_velocity.second > 0) {
			(*m_loptica)[i].m_velocity.second = -(*m_loptica)[i].m_velocity.second;
			change = true;
		}

		(*m_loptica)[i].m_c_in_row += (change == true ? 1 : 0);
		if ((*m_loptica)[i].m_c_in_row > 15) {
			(*m_loptica)[i].m_velocity.first += 1;
			if ((*m_loptica)[i].m_velocity.first > 3)
				(*m_loptica)[i].m_velocity.first = 3;
			(*m_loptica)[i].m_c_in_row = 0;
		}

	}

	doDamage();

	size_t sz = ispali.size();
	if (sz == 0)
		return change;
	sz -= 1;
	for (size_t i = sz; i >= 0 && i <= 50; i--) {
		//std::cout << sz << " " << i << std::endl;
		(*m_loptica).erase((*m_loptica).begin() + ispali[i]);
	}

	return change;

}

void LDZ_CollisionManager::doDamage() {

	while (Hit.size() > 0) {

		bool Destroyed = m_level.m_LevelLayout[Hit[Hit.size() - 1].first][Hit[Hit.size() - 1].second].Hit();
		if (Destroyed) {

			if (m_level.m_LevelLayout[Hit[Hit.size() - 1].first][Hit[Hit.size() - 1].second].m_special == true) {
				(*m_loptica).emplace_back(m_renderer, "./Textures/Ball/ball.png");
				//std::cout << (*m_loptica)[1].m_velocity.first << " "  << (*m_loptica)[1].m_velocity.second << std::endl;
				//std::cout << (*m_loptica)[0].m_velocity.first << " " << (*m_loptica)[0].m_velocity.second << std::endl;
			}

			m_level.m_TextureLayout[Hit[Hit.size() - 1].first][Hit[Hit.size() - 1].second].Destroy();
			m_level.m_totalBlocks--;
			m_level.m_mixer.PlayEffect(m_level.m_LevelLayout[Hit[Hit.size() - 1].first][Hit[Hit.size() - 1].second].m_BreakSoundIndex);
		
			if (m_level.m_totalBlocks == 0) {
				LoadNextLevel = true;
			}
		}
		else {
			m_level.m_mixer.PlayEffect(m_level.m_LevelLayout[Hit[Hit.size() - 1].first][Hit[Hit.size() - 1].second].m_HitSoundIndex);
		}
			
		Hit.pop_back();
	}
	return;
}
