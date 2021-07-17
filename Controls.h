#pragma once
#include <SDL.h>

struct MoveCommand
{
	bool forward{ 0 }, back{ 0 }, left{ 0 }, right{ 0 }, quit{ 0 }, sleft{ 0 }, sright{ 0 };
};

struct Player
{
	SDL_FPoint pos;
	float dir;
	int NoseLength{ 10 };
	void Updatepos(MoveCommand cmd, float speed = 0.63f, float turnRate = 0.032f);
};

void Gather_inputs(MoveCommand*);
