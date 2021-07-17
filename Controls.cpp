#include "Controls.h"
#include <cmath>

void Player::Updatepos(MoveCommand cmd, float speed, float turnRate)
{
	if ( cmd.left && !cmd.right ) dir += turnRate;
	if ( !cmd.left && cmd.right ) dir -= turnRate;
	if ( cmd.forward && !cmd.back )
	{
		pos.x += std::cos(dir) * speed;
		pos.y -= std::sin(dir) * speed;
	}
	if ( cmd.back && !cmd.forward )
	{
		pos.x -= std::cos(dir) * speed;
		pos.y += std::sin(dir) * speed;
	}
	if ( cmd.sleft && !cmd.sright )
	{
		pos.x -= std::sin(dir) * speed;
		pos.y -= std::cos(dir) * speed;
	}
	if ( !cmd.sleft && cmd.sright )
	{
		pos.x += std::sin(dir) * speed;
		pos.y += std::cos(dir) * speed;
	}
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
			}
		}
	}
}
