#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

#include "renderwindow.h"
#include "entity.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;

RenderWindow::RenderWindow()
{

}

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, p_w, p_h, 0);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	if (window == NULL)
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;

	renderer = SDL_CreateRenderer(window, NULL);
}


SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

void RenderWindow::clear()
{
	SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{

	for (int i = p_entity.getSize() - 1; i >= 0; i--)
	{
		SDL_FRect src;
		src.x = p_entity.getCurrentFrame().x;
		src.y = p_entity.getCurrentFrame().y;
		src.w = p_entity.getCurrentFrame().w;
		src.h = p_entity.getCurrentFrame().h;

		SDL_FRect dst;
		dst.x = p_entity.getX() + p_entity.getAnimOffsetX(i);
		dst.y = p_entity.getY() + p_entity.getAnimOffsetY(i);
		dst.w = p_entity.getCurrentFrame().w;
		dst.h = p_entity.getCurrentFrame().h;

		SDL_RenderTexture(renderer, p_entity.getTex(i), &src, &dst);
	}
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex)
{
	SDL_FRect src;
	src.x = 0;
	src.y = 0;
	SDL_GetTextureSize(p_tex, &src.w, &src.h);

	SDL_FRect dst;
	dst.x = p_x;
	dst.y = p_y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderTexture(renderer, p_tex, &src, &dst);
}

void RenderWindow::render(SDL_Texture* p_tex)
{
	render(0, 0, p_tex);
}

void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, 0, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_FRect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h;

		SDL_FRect dst;
		dst.x = p_x;
		dst.y = p_y;
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderTexture(renderer, message, &src, &dst);
		SDL_DestroySurface(surfaceMessage);
}

void RenderWindow::renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, p_text, 0, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_FRect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h;

		SDL_FRect dst;
		dst.x = SCREEN_WIDTH/2 - src.w/2 + p_x;
		dst.y = SCREEN_HEIGHT/2 - src.h/2 + p_y;
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderTexture(renderer, message, &src, &dst);
		SDL_DestroySurface(surfaceMessage);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
}
