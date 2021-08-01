#include <cmath>
#include "Controls.h"
#include "Renderings.h"
#include "Utils.h"

Player::Player(float x, float y, float d) : pos{ x,y }, dir{ d } {}

SDL_FPoint Player::Collide(SDL_FPoint attempt, const std::vector<Wall>& walls)
{
	for ( Wall wall : walls )
	{
		SDL_FPoint wallDir{ wall.end2 - wall.end1 };
		wallDir *= 1 / std::sqrt(wallDir.x*wallDir.x + wallDir.y*wallDir.y);
		wall.end1 -= radius * wallDir;
		wall.end2 += radius * wallDir;
		SDL_FPoint step{ attempt - pos };

		if ( TestForIntersection(wall.end1 - pos, wall.end2 - pos, step) > IntersectType::second_out )
		{
			//std::cout << "not moving towards a wall\n";
			//std::cout << wallDir.x << ' ' << wallDir.y << "\n";
			continue;
		}
		//std::cout << wallDir.x <<' '<<wallDir.y <<"\n";

		SDL_FPoint normal = LeftFace(wallDir);
		float distOld = dot(pos - wall.end1, normal);
		float distNew = dot(attempt - wall.end1, normal);

		if ( distOld < 0.f )
		{
			distOld = -distOld;
			distNew = -distNew;
			//normal = -normal;
		}

		if ( distNew < radius && distNew < distOld )
		{
			attempt = pos  + dot(wallDir, step) * wallDir;
			//attempt = pos;
			//std::cout << "Nevaer1\n";
		}
	}

	return attempt;
}

void Player::Updatepos(MoveCommand cmd, const std::vector<Wall>& walls, float speed, float turnRate)
{
	if ( cmd.turbo )
	{
		speed *= 15;
		turnRate *= 2;
	}
	if ( cmd.left && !cmd.right ) dir += turnRate;
	if ( !cmd.left && cmd.right ) dir -= turnRate;

	SDL_FPoint newpos{ pos };
	float sin{ std::sin(dir) }, cos{ std::cos(dir) };

	if ( cmd.forward && !cmd.back )
	{
		newpos.x += cos * speed;
		newpos.y -= sin * speed;
	}
	if ( cmd.back && !cmd.forward )
	{
		newpos.x -= cos * speed;
		newpos.y += sin * speed;
	}
	if ( cmd.sleft && !cmd.sright )
	{
		newpos.x -= sin * speed;
		newpos.y -= cos * speed;
	}
	if ( !cmd.sleft && cmd.sright )
	{
		newpos.x += sin * speed;
		newpos.y += cos * speed;
	}

	pos = Collide(newpos, walls);
}

void Gather_inputs(MoveCommand* moves)
{
	for ( SDL_Event event; !moves->quit && SDL_PollEvent(&event); )
	{
		switch ( event.type )
		{
		case SDL_QUIT:
			moves->quit = true; break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			switch ( event.key.keysym.sym )
			{
			case SDLK_ESCAPE:
				moves->quit = true; break;
			case SDLK_UP:
			case SDLK_w:
				moves->forward = event.type == SDL_KEYDOWN; break;
			case SDLK_DOWN:
			case SDLK_s:
				moves->back = event.type == SDL_KEYDOWN; break;
			case SDLK_a:
				moves->sleft = event.type == SDL_KEYDOWN; break;
			case SDLK_d:
				moves->sright = event.type == SDL_KEYDOWN; break;
			case SDLK_LEFT:
			case SDLK_q:
				moves->left = event.type == SDL_KEYDOWN; break;
			case SDLK_RIGHT:
			case SDLK_e:
				moves->right = event.type == SDL_KEYDOWN; break;
			case SDLK_LSHIFT:
				moves->turbo = event.type == SDL_KEYDOWN; break;
			}
		}
	}
}
