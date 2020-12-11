// C++ headers //
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>

// SDL headers //
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>


// GLI headers //


// TinyXML parser //
#include <tinyxml2.h>


// My headers //
#include <LDZ_Window.h>
#include <LDZ_Renderer.h>
#include <LDZ_Texure.h>
#include <LDZ_Level.h>
#include <LDZ_Palica.h>
#include <LDZ_Loptica.h>
#include <LDZ_Text.h>
#include <LDZ_Mixer.h>
#include <LDZ_CollisionManager.h>

const int SCREEN_WIDTH = 793;
const int SCREEN_HEIGHT = 413;

unsigned numberOfDigits(unsigned i);

int curr_level = 0;
bool LoadNextLevel = false;
unsigned int Lifes = 5;
unsigned int Score = 0;
bool PauseNextLife = false;

int main(int argc, char* args[]) {


	// SDL initialization // 

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL_init error" << std::endl;
		return 1;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cout << "IMG_init error" << std::endl;
		return 1;
	}

	if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3) {
		std::cout << "MIX_init error" << std::endl;
		return 1;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		std::cout << "MIX_openaudio error" << " " << Mix_GetError() << std::endl;
		return 1;
	}

	{ // Gameloop stack frame //
	
	// Load Level //
	std::vector<LDZ_Level> Game;
	Game.reserve(10);
	Game.emplace_back("level.xml");
	Game.emplace_back("level1.xml");
	Game.emplace_back("level2.xml");
	Game.emplace_back("level3.xml");
	Game.emplace_back("level4.xml");
	LDZ_Window window("Breakout - Luka Dzimbeg", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	LDZ_Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	LDZ_Palica player(renderer, "Textures/Player/player.png");
	std::vector<LDZ_Loptica> loptice;
	loptice.reserve(10);
	loptice.emplace_back(renderer, "./Textures/Ball/ball.png");
	LDZ_Texture GameOverScreen(renderer, "./Textures/Screen/GameOver.png");
	LDZ_Texture YouWinScreen(renderer, "./Textures/Screen/YouWin.png");
	LDZ_Texture LifeSymbol(renderer, "./Textures/Other/heart.png");


	//Tutorial kratki//
	LDZ_Texture Tutorial(renderer, "./Textures/Boards/Tutorial.png");
	renderer.copy(Tutorial);
	renderer.present();
	SDL_Delay(7000);
	/////


	LDZ_CollisionManager* Collider = new LDZ_CollisionManager(Game[0], player, &loptice, renderer);
	LDZ_Text text(renderer, "./Textures/Font/font.png");



	Game[0].LoadTextures(renderer);
	Game[1].LoadTextures(renderer);
	Game[2].LoadTextures(renderer);
	Game[3].LoadTextures(renderer);
	Game[4].LoadTextures(renderer);
	renderer.clear();
	LifeSymbol.set_dst(55, 25, 35, 35);
	renderer.copy(Game[0].m_Background);
	renderer.copy(LifeSymbol);
	renderer.copy(player);
	for(int i = 0; i < loptice.size(); i++)
		renderer.copy(loptice[i]);
	renderer.copy(Game[0]);
	renderer.present();

	bool LeftDown = false, RightDown = false;
	while (true) {

		if (Lifes == 0) {
			GameOverScreen.set_dst(-1, -1, -1, -1);
			renderer.clear();
			renderer.copy(GameOverScreen);
			text.Type(315, 200, "GAME OVER");
			text.Type(350 - numberOfDigits(Score) * 10, 230, "SCORE " + std::to_string(Score));
			renderer.present();
			SDL_Delay(1500);
			break;
		}

		if (LoadNextLevel == true) {
			LoadNextLevel = false;
			curr_level++;
			delete Collider;
			if (curr_level == Game.size()) {
				YouWinScreen.set_dst(-1, -1, -1, -1);
				renderer.clear();
				renderer.copy(YouWinScreen);
				text.Type(325, 175, "YOU WON");
				text.Type(335 - numberOfDigits(Score) * 10, 205, "SCORE " + std::to_string(Score));
				renderer.present();
				SDL_Delay(1500);
				break;
			}
			Collider = new LDZ_CollisionManager(Game[curr_level], player, &loptice , renderer);

		}
			

		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				break;
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
				RightDown = false;
				LeftDown = true;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
				RightDown = true;
				LeftDown = false;
			}
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT) {
				LeftDown = false;
			}
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT) {
				RightDown = false;
			}
		}

		if (LeftDown)
			player.setDst(-5);
		else if (RightDown)
			player.setDst(5);

		Collider->detect(LeftDown , RightDown);
		for (int i = 0; i < loptice.size(); i++)
			loptice[i].update();
		//----------------------------//
		renderer.clear();
		renderer.copy(Game[curr_level].m_Background);
		renderer.copy(LifeSymbol);
		renderer.copy(player);
		for (int i = 0; i < loptice.size(); i++)
			renderer.copy(loptice[i]);
		renderer.copy(Game[curr_level]);
		text.Type(30, 29, std::to_string(Lifes));
		text.Type(12, 153, "SCORE");
		text.Type(12, 358, "LEVEL");
		text.Type(52, 382, std::to_string(curr_level));
		text.Type(16 + (4 - numberOfDigits(Score))*10, 180, std::to_string(Score));

		renderer.present();

		if (PauseNextLife == true) {
			PauseNextLife = false;
			SDL_Delay(1500);
		}

		SDL_Delay(10);

	}

	//DeleteTextures
	GameOverScreen.Destroy();
	LifeSymbol.Destroy();

	} // Gameloop stack frame END //

	


	// EXIT //

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}


unsigned numberOfDigits(unsigned i)
{
	return i > 0 ? (int)log10((double)i)+ 1 : 1;
}