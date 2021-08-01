#pragma once
#include <SDL.h>
#include <vector>
//#include "Renderings.h"

struct MoveCommand
{
	bool forward{ 0 }, back{ 0 }, left{ 0 }, right{ 0 },
		quit{ 0 }, sleft{ 0 }, sright{ 0 }, turbo{ 0 };
};

struct Wall;

struct Player
{
	SDL_FPoint pos;
	float dir;
	int NoseLength{ 10 };
	float radius{ 4 };

	Player(float x, float y, float d);

	void Updatepos(
		MoveCommand cmd,
		const std::vector<Wall>& walls,
		float speed = 0.63f,
		float turnRate = 0.032f
	);
private:
	SDL_FPoint Collide(SDL_FPoint attempt, const std::vector<Wall>& walls);
};

void Gather_inputs(MoveCommand*);
