#include <cmath>
#include <algorithm>
#include "Controls.h"
#include "Renderings.h"
#include "Utils.h"

Player::Player(float x, float y, float d) : pos{ x,y }, dir{ d } {}

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


// Secret private and undeclared stuff

struct Collision
{
	const Wall* p_wall{ nullptr };
	SDL_FPoint wallDir{};
	SDL_FPoint awayNormal{};
	float t{ 0.f };
	bool operator<(const Collision& o) { return t < o.t; }
};

static std::vector<Collision> Collisions(6);

bool SphereCast(
	SDL_FPoint castStart,
	SDL_FPoint step,
	float radius,
	const std::vector<Wall>& walls,
	std::vector<Collision>& results)
{
	results.clear();

	for ( const Wall& w : walls )
	{
		SDL_FPoint castNormal = LeftFace(step);
		float stepLen = std::sqrt(dot(step, step));
		castNormal *= 1 / stepLen;
		SDL_FPoint end1rel = w.end1-castStart;
		SDL_FPoint end2rel = w.end2-castStart;
		float crossrange1 = dot(end1rel, castNormal);
		float crossrange2 = dot(end2rel, castNormal);
		if ( crossrange1 > radius && crossrange2 > radius ) continue;
		if ( crossrange1 < -radius && crossrange2 < -radius ) continue;


		// Check for endpoints

		auto fromPoint = [&w](SDL_FPoint end, SDL_FPoint p, float t)->Collision
		{
			Collision res;
			res.p_wall = &w;
			res.t = t;
			res.awayNormal = Normalized(p - end);
			res.wallDir = LeftFace(res.awayNormal);
			return res;
		};

		float r2 = radius * radius;
		if ( crossrange1 <= radius && crossrange1 >= -radius )
		{
			if ( Norm2(end1rel) <= r2 )
			{
				Collisions.push_back(fromPoint(w.end1, castStart, 0));
				//continue;
			}

			float distance = dot(end1rel, step) / stepLen;
			if ( (0 <= distance && distance <= stepLen) || (Norm2(end1rel - step) <= r2) )
			{
				float sin = crossrange1 / radius;
				float cos = std::sqrt(1 - sin * sin);
				float t = (distance - cos * radius) / stepLen;
				auto point = castStart + t * step;
				Collisions.push_back(fromPoint(w.end1, point, t));
				//continue;
			}
		}

		if ( crossrange2 <= radius && crossrange2 >= -radius )
		{
			if ( Norm2(end2rel) <= r2 )
			{
				Collisions.push_back(fromPoint(w.end2, castStart, 0));
				//continue;
			}

			float distance = dot(end2rel, step) / stepLen;
			if ( (0 <= distance && distance <= stepLen) || (Norm2(end2rel - step) <= r2) )
			{
				float sin = crossrange2 / radius;
				float cos = std::sqrt(1 - sin * sin);
				float t = (distance - cos * radius) / stepLen;
				auto point = castStart + t * step;
				Collisions.push_back(fromPoint(w.end2, point, t));
				//continue;
			}
		}


		// Check for points on the line straight ahead

		SDL_FPoint wallDir{ w.end2 - w.end1 };
		Normalize(wallDir);
		SDL_FPoint wallNormal = LeftFace(wallDir);

		float startProj = dot(wallNormal, castStart - w.end1);
		float p = dot(step, wallNormal);
		if ( startProj < 0 )
		{
			wallNormal = -wallNormal;
			startProj += radius;
		}
		else { startProj -= radius; }

		float t = -startProj / p;

		if ( t >= 0 && t <= 1 )
		{
			results.push_back({ &w, wallDir, wallNormal, t });
		}
	}

	return results.size();
}

SDL_FPoint Player::Collide(SDL_FPoint attempt, const std::vector<Wall>& walls)
{
	//Collisions.clear();
	/*
	for ( const Wall& wall : walls )
	{
		SDL_FPoint wallDir{ wall.end2 - wall.end1 };
		Normalize(wallDir);
		SDL_FPoint end1 = wall.end1 - radius * wallDir;
		SDL_FPoint end2 = wall.end2 + radius * wallDir;
		//SDL_FPoint step = attempt - pos;

		if ( TestForIntersection(end1 - pos, end2 - pos, step) > IntersectType::second_out )
		{
			continue;
		}

		SDL_FPoint normal = LeftFace(wallDir);
		float distOld = dot(pos - end1, normal);
		float distNew = dot(attempt - end1, normal);

		if ( distOld < 0.f )
		{
			distOld = -distOld;
			distNew = -distNew;
			//normal = -normal;
		}

		if ( distNew < radius && distNew < distOld )
		{
			//attempt = pos + dot(wallDir, step) * wallDir;
			//attempt = pos;
			Collisions.emplace_back(&wall, wallDir, distOld);
		}
	}
	std::sort(Collisions.begin(), Collisions.end(),
		[](Collision& a, Collision& b) { return a.t < b.t; });
	for ( auto& c : Collisions )
	{
		step = attempt - pos;
		attempt = pos + dot(c.wallDir, step) * c.wallDir;
	}
	*/
	
	SDL_FPoint step = attempt - pos;
	if ( SphereCast(pos, step, radius, walls, Collisions) )
	{
		std::sort(Collisions.begin(), Collisions.end());
		Collision* c = &Collisions.front();

		SDL_FPoint intermediate = pos + step * c->t + c->awayNormal * 0.001f;
		step = ((1.f - c->t) * dot(c->wallDir, step)) * c->wallDir;

		if ( SphereCast(intermediate, step, radius, walls, Collisions) )
		{
			std::sort(Collisions.begin(), Collisions.end());
			intermediate += step * Collisions.front().t;
		}
		else
		{
			intermediate += step;
		}

		return intermediate;
	}

	return attempt;
}
