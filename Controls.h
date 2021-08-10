#pragma once
#include <SDL.h>
#include <vector>

struct MoveCommand
{
	bool forward{ 0 }, back{ 0 }, left{ 0 }, right{ 0 },
		quit{ 0 }, sleft{ 0 }, sright{ 0 }, turbo{ 0 }, slow{ 0 };
};

struct Wall
{
	SDL_FPoint end1, end2;
	SDL_Color color;
	Wall(float x1, float y1, float x2, float y2, SDL_Color clr);
		//:end1{ x1,y1 }, end2{ x2,y2 }, color{ clr } {}
};

class Player
{
public:
	SDL_FPoint pos;
	float dir;
	int NoseLength{ 10 };
	float radius{ 13.0f };
	//float radius{ 0.001f };

	Player(float x, float y, float d);

	void Updatepos(
		MoveCommand cmd,
		const std::vector<Wall>& walls,
		float speed = 0.63f,
		float turnRate = 0.02f
	);

	SDL_FPoint Collide(SDL_FPoint step, const std::vector<Wall>& walls) const;
};

void Gather_inputs(MoveCommand*);

struct Collision
{
	const Wall* p_wall;
	SDL_FPoint contactPoint;
	SDL_FPoint wallDir;
	SDL_FPoint normal;
	float t;
	float distSq;

	Collision() = default;
	Collision(
		const Wall* wallPtr,
		SDL_FPoint contactPoint,
		SDL_FPoint wallDir,
		SDL_FPoint normal,
		float t,
		float distSq);
		//: p_wall{ wallPtr }, contactPoint{ contactPoint },
		//wallDir{ wallDir }, normal{ normal }, t{ t }, distSq{ distSq } {}
};


bool SphereCast(
	SDL_FPoint castStart,
	SDL_FPoint step,
	float radius,
	const std::vector<Wall>& walls,
	std::vector<Collision>& results
);
