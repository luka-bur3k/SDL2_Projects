#include <LDZ_Renderer.h>

void LDZ_Renderer::clear() {
	SDL_RenderClear(m_renderer);
}

void LDZ_Renderer::copy(LDZ_Texture n_tex) {
	SDL_RenderCopy(m_renderer, n_tex, n_tex.get_src() , n_tex.get_dst());
}

void LDZ_Renderer::copy(LDZ_Level&p_level) {


	auto p_LevelLayout = p_level.getLevelLayout();
	auto p_TextureLayout = p_level.getTextureLayout();

	for (int i = 0; i < p_TextureLayout.size(); i++) {
		for (int j = 0; j < p_TextureLayout[i].size(); j++) {
			
			if ((SDL_Texture*)p_TextureLayout[i][j] == nullptr)
				continue;
			SDL_RenderCopy(m_renderer, p_TextureLayout[i][j], NULL, p_TextureLayout[i][j].get_dst());
			//std::cout << SDL_GetError() << std::endl;
			//system("pause");
		}
	}
}

void LDZ_Renderer::present() {
	SDL_RenderPresent(m_renderer);
}