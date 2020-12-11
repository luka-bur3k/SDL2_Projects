#include <LDZ_Level.h>


LDZ_Level::LDZ_Level(const char* p_filepath) {
	
	std::stringstream ss;

	m_totalBlocks = 0;
	tinyxml2::XMLDocument p_doc;
	p_doc.LoadFile(p_filepath);

	m_RowCount = m_ColumnSpacing = m_ColumnCount = m_RowSpacing = 0;
	
	std::string temp = p_doc.FirstChildElement()->Attribute("RowCount");
	ss << temp;
	ss >> m_RowCount;
	ss.str("");
	ss.clear();
	//std::cout << "RowCount : " << m_RowCount << "\n";


	temp = p_doc.FirstChildElement()->Attribute("ColumnCount");
	ss << temp;
	ss >> m_ColumnCount;
	ss.str("");
	ss.clear();
	//std::cout << "ColumnCount : " << m_ColumnCount << "\n";

	temp = p_doc.FirstChildElement()->Attribute("RowSpacing");
	ss << temp;
	ss >> m_RowSpacing;
	ss.str("");
	ss.clear();
	//std::cout << "RowSpacing : " << m_RowSpacing << "\n";

	temp = p_doc.FirstChildElement()->Attribute("ColumnSpacing");
	ss << temp;
	ss >> m_ColumnSpacing;
	ss.str("");
	ss.clear();
	//std::cout << "ColumnSpacing : " << m_ColumnSpacing << "\n";

	m_BackgroundTexture = p_doc.FirstChildElement()->Attribute("BackgroundTexture");
	m_PlayerTexture = p_doc.FirstChildElement()->Attribute("PlayerTexture");
	//std::cout << "BackgroundTexture : " << m_BackgroundTexture << "\n";
	//std::cout << p_doc.FirstChildElement()->Attribute("BackgroundMusic") << std::endl;
	m_mixer.AddToMix(p_doc.FirstChildElement()->Attribute("BackgroundMusic") , true);



	std::vector<LDZ_Texture> m_fill;
	for (int i = 0; i < m_ColumnCount; i++) {
		m_LevelLayout.push_back(m_BrickTypes);
		m_TextureLayout.push_back(m_fill);
	}
	// moglo se izvest i uz 1d vector no sta je tu je manje koda i ljepse vizulno 
	
	tinyxml2::XMLElement* elem = p_doc.FirstChildElement()->FirstChildElement()->FirstChildElement();
	while (elem != nullptr) {

		m_BrickTypes.emplace_back(elem->Attribute("Id"), elem->Attribute("Texture"), elem->Attribute("HitPoints"), 
			elem->Attribute("HitSound"), elem->Attribute("BreakSound"), elem->Attribute("BreakScore"));
		
		m_BrickTypes[m_BrickTypes.size() - 1].setIndeces(m_mixer.AddToMix(elem->Attribute("HitSound")), m_mixer.AddToMix(elem->Attribute("BreakSound")));
		
		elem = elem->NextSiblingElement();
	//	std::cout << "Iteracija\n";
	}
	
	m_BrickTypes.emplace_back('.', nullptr, 0, nullptr, nullptr, 0);

	elem = p_doc.FirstChildElement()->FirstChildElement()->NextSiblingElement();
	std::string layout = elem->GetText();

	//for (int i = 0; i < layout.size(); i++)
	//	std::cout << int(layout[i]) << " " << std::endl;

	
	 
	auto findBrickType = [&](char p_id) -> LDZ_Bricks {
		for (int i = 0; i < m_BrickTypes.size(); i++) {
			if (m_BrickTypes[i].getId() == p_id)
				return m_BrickTypes[i];
		}
		return m_BrickTypes[m_BrickTypes.size() - 1];
	};

	

	for (int i = 1, j = 0; i < layout.size(); i++) {
	
		if (layout[i] == 95) {
			m_LevelLayout[j].push_back(m_BrickTypes[m_BrickTypes.size() - 1]);
		
			j++;
		}	
		else if ((layout[i] >= 65 && layout[i] <= 90) || (layout[i] >= 97 && layout[i] <= 122)) {
			m_LevelLayout[j].push_back(findBrickType(layout[i]));
			j++;
		
		}
	

		if (j == m_ColumnCount) {
			j = 0;
		}
			
	}

}


void LDZ_Level::LoadTextures(SDL_Renderer* p_renderer) {

	int size_x = (660 - m_ColumnSpacing * (1 + m_ColumnCount)) / m_ColumnCount;
	int size_y = (390 - m_RowSpacing * (1 + m_RowCount)) / m_RowCount;

	for (int i = 0; i < m_LevelLayout.size(); i++) {
		for (int j = 0; j < m_LevelLayout[i].size(); j++) {
			std::string temp = m_LevelLayout[i][j].getTexture();
			//std::cout << temp << std::endl;
			if (temp != "") {
				m_TextureLayout[i].emplace_back(p_renderer, temp.c_str());
				if(m_LevelLayout[i][j].m_HitPoints <= 9000)
					m_totalBlocks++;
				int temp_x = 120 + i * (size_x + m_ColumnSpacing);
				int temp_y = 13 + j * (size_y + m_RowSpacing);
				m_TextureLayout[i][j].set_dst(temp_x, temp_y, size_y, size_x);

			}
				
			else 
				m_TextureLayout[i].emplace_back(p_renderer, nullptr);
		}
	}
	m_Background.set_Texture(p_renderer, m_BackgroundTexture.c_str());

}

void LDZ_Level::reset_Level() {
	Lifes--;
}