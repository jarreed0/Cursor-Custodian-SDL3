#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>

#include "entity.h"
#include "ground.h"

class Player : public Entity
{
public:
	Player() {}
	Player(float p_x, float p_y, std::vector<SDL_Texture*> p_tex);
	float distanceFromCursor();
	bool jump();
	void update(Ground& ground);
	std::string getScore();
	std::string getHighscore();
	int getScoreInt();
	int isDead();
	void reset();
private:
	float velocityX, velocityY;
	bool grounded;
	void animEyes();
	float clamp(float p_value, float p_min, float p_max);
	int score = 0;
	int highscore = 0;
	int timer = 0;
	int dead = 0;
};
