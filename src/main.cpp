#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <stdlib.h>

#include "renderwindow.h"
#include "entity.h"
#include "player.h"
#include "ground.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

const int ALIVE = 0;
const int CURSOR_DEATH = 1;
const int HOLE_DEATH = 2;

const Uint8 *keyState;

RenderWindow window;

std::vector<SDL_Texture*> playerTex;
SDL_Texture* groundTex[4];
SDL_Texture* arrow;
SDL_Texture* highscoreBox;
SDL_Texture* deathOverlay;
SDL_Texture* logo;

TTF_Font* font32;
TTF_Font* font32_outline;
TTF_Font* font24;
TTF_Font* font16;

SDL_Color white = { 255, 255, 255 };
SDL_Color black = { 0, 0, 0 };

Mix_Chunk* jumpSfx;
Mix_Chunk* fallSfx;
Mix_Chunk* hitSfx;
Mix_Chunk* clickSfx;

bool playedDeathSFX = false;
bool mainMenu = true;

SDL_AudioSpec spec;

Player player = Player();
Ground ground = Ground();

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	TTF_Init();

	window.create("Cursor Custodian", SCREEN_WIDTH, SCREEN_HEIGHT);

	spec.freq = 44100;
	spec.format = MIX_DEFAULT_FORMAT;
	spec.channels = 2;
	Mix_OpenAudio(0, &spec);

	srand((unsigned)time(0));

	playerTex.push_back(window.loadTexture("res/textures/player/player_0.png"));
	playerTex.push_back(window.loadTexture("res/textures/player/player_1.png"));
	playerTex.push_back(window.loadTexture("res/textures/player/player_2.png"));
	playerTex.push_back(window.loadTexture("res/textures/player/player_3.png"));
	playerTex.push_back(window.loadTexture("res/textures/player/player_4.png"));

	groundTex[0] = window.loadTexture("res/textures/ground/left.png");
	groundTex[1] = window.loadTexture("res/textures/ground/center.png");
	groundTex[2] = window.loadTexture("res/textures/ground/right.png");
	groundTex[3] = window.loadTexture("res/textures/ground/hole.png");

	arrow = window.loadTexture("res/textures/arrow.png");
	highscoreBox = window.loadTexture("res/textures/highscore_box.png");
	deathOverlay = window.loadTexture("res/textures/death_overlay.png");
	logo = window.loadTexture("res/textures/logo.png");

	font32 = TTF_OpenFont("res/fonts/cocogoose.ttf", 32);
	font32_outline = TTF_OpenFont("res/fonts/cocogoose.ttf", 32);
	font24 = TTF_OpenFont("res/fonts/cocogoose.ttf", 24);
	font16 = TTF_OpenFont("res/fonts/cocogoose.ttf", 16);
	TTF_SetFontOutline(font32_outline, 3);

	jumpSfx = Mix_LoadWAV("res/sounds/jump.wav");
	fallSfx = Mix_LoadWAV("res/sounds/fall.wav");
	hitSfx = Mix_LoadWAV("res/sounds/hit.wav");
	clickSfx = Mix_LoadWAV("res/sounds/click.wav");
	Mix_PlayChannel(-1, jumpSfx, 0);

	player = Player(0, 0, playerTex);
	ground = Ground(groundTex[0], groundTex[1], groundTex[2], groundTex[3]);

	return SDL_APP_CONTINUE;
}


void reset()
{
	player.reset();
	ground.reset();
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	if (mainMenu)
	{
		if (SDL_GetTicks() < 2500)
		{
			window.clear();
			window.renderCenter(0, sin(SDL_GetTicks()/100) * 2 - 4, "POLYMARS", font24, white);
			window.display();
		}
		else
		{
			window.clear();

			window.render(SCREEN_WIDTH/2 - 234, SCREEN_HEIGHT/2 - 94 - 30, logo);
			window.renderCenter(0, 90 + sin(SDL_GetTicks()/100) * 2, "Click to start", font24, white);

			for (int i = 0; i < ground.getLength(); i++)
			{
				window.render(ground.getTile(i));
			}
			window.display();
		}
	}
	else
	{
		if (player.isDead() != CURSOR_DEATH)
		{
			ground.update(player.getScoreInt());
		}

		if (player.isDead() == ALIVE)
		{
			player.update(ground);
		}
		else if (!playedDeathSFX)
		{
			if (player.isDead() == CURSOR_DEATH)
			{
				Mix_PlayChannel(-1, hitSfx, 0);
			}
			else if (player.isDead() == HOLE_DEATH)
			{
				Mix_PlayChannel(-1, fallSfx, 0);
			}
			playedDeathSFX = true;
		}

		window.clear();

		window.render(player);
		for (int i = 0; i < ground.getLength(); i++)
		{
			window.render(ground.getTile(i));
		}
		window.render(25, 30, arrow);
		window.render(62, 20, player.getScore().c_str(), font32_outline, black);
		window.render(65, 23, player.getScore().c_str(), font32, white);
		window.render(0, 65, highscoreBox);
		window.render(65, 64, player.getHighscore().c_str(), font16, white);

		if (player.isDead() != ALIVE)
		{
			window.render(deathOverlay);
			if (player.isDead() == CURSOR_DEATH)
			{
				window.renderCenter(0, -24, "The cursor is deadly to the player...", font24, white);
			}
			else if (player.isDead() == HOLE_DEATH)
			{
				window.renderCenter(0, -24, "The hole had no bottom...", font24, white);
			}
			window.renderCenter(0, 12, "Click to retry.", font16, white);
		}
		window.display();
	}
    	SDL_Delay(16);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
	switch (event->type)
	{
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
	case SDL_EVENT_KEY_DOWN:
		switch (event->key.scancode)
		{
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Q:
				return SDL_APP_SUCCESS;
			default:
				break;
		}
    	case SDL_EVENT_MOUSE_BUTTON_DOWN:
    		if (mainMenu)
    		{
    			if (event->button.button == SDL_BUTTON_LEFT && SDL_GetTicks() > 2500)
    			{
    				mainMenu = false;
    				Mix_PlayChannel(-1, clickSfx, 0);
    			}
    		}
    		else
    		{
    			if (event->button.button == SDL_BUTTON_LEFT && player.isDead() == ALIVE)
    			{
				if (player.jump())
						{
						Mix_PlayChannel(-1, jumpSfx, 0);
					}
    			}
    			else if (player.isDead() != ALIVE)
    			{
    				Mix_PlayChannel(-1, clickSfx, 0);
    				reset();
    				playedDeathSFX = false;
    			}
    		}
		break;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
		window.cleanUp();
		TTF_CloseFont(font32);
		TTF_CloseFont(font32_outline);
		TTF_CloseFont(font24);
		TTF_CloseFont(font16);
		TTF_Quit();
}
