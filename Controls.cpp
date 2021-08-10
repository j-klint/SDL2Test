#include <cmath>
#include <algorithm>
#include "Controls.h"
#include "Renderings.h"
#include "Utils.h"

static std::vector<Collision> Collisions(6);
static std::vector<Collision*> CollPtrs(6);

Wall::Wall(float x1, float y1, float x2, float y2, SDL_Color clr)
	:end1{ x1,y1 }, end2{ x2,y2 }, color{ clr } {}

Player::Player(float x, float y, float d) : pos{ x,y }, dir{ d } {}

Collision::Collision(
	const Wall* wallPtr,
	SDL_FPoint contactPoint,
	SDL_FPoint wallDir,
	SDL_FPoint normal,
	float t,
	float distSq)
	: p_wall{ wallPtr }, contactPoint{ contactPoint },
	wallDir{ wallDir }, normal{ normal }, t{ t }, distSq{ distSq } {}

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
			//case SDLK_UP:
			case SDLK_w:
				moves->forward = event.type == SDL_KEYDOWN; break;
			//case SDLK_DOWN:
			case SDLK_s:
				moves->back = event.type == SDL_KEYDOWN; break;
			case SDLK_a:
				moves->sleft = event.type == SDL_KEYDOWN; break;
			case SDLK_d:
				moves->sright = event.type == SDL_KEYDOWN; break;
			//case SDLK_LEFT:
			case SDLK_q:
				moves->left = event.type == SDL_KEYDOWN; break;
			//case SDLK_RIGHT:
			case SDLK_e:
				moves->right = event.type == SDL_KEYDOWN; break;
			case SDLK_LSHIFT:
				moves->turbo = event.type == SDL_KEYDOWN; break;
			case SDLK_LCTRL:
				moves->slow = event.type == SDL_KEYDOWN; break;
			}
		}
	}
}

void Player::Updatepos(MoveCommand cmd, const std::vector<Wall>& walls, float speed, float turnRate)
{
	if ( cmd.turbo && !cmd.slow )
	{
		speed *= 15;
		turnRate *= 3;
	}
	if ( !cmd.turbo && cmd.slow )
	{
		speed /= 5;
		turnRate /= 5;
	}
	if ( cmd.left && !cmd.right ) dir += turnRate;
	if ( !cmd.left && cmd.right ) dir -= turnRate;

	SDL_FPoint step{};
	float sin{ std::sin(dir) * speed }, cos{ std::cos(dir) * speed };

	if ( cmd.forward && !cmd.back )
	{
		step.x += cos;
		step.y -= sin;
	}
	if ( cmd.back && !cmd.forward )
	{
		step.x -= cos;
		step.y += sin;
	}
	if ( cmd.sleft && !cmd.sright )
	{
		step.x -= sin;
		step.y -= cos;
	}
	if ( !cmd.sleft && cmd.sright )
	{
		step.x += sin;
		step.y += cos;
	}

	pos = Collide(step, walls);
}

SDL_FPoint Player::Collide(SDL_FPoint step, const std::vector<Wall>& walls) const
{
	SDL_FPoint unstuckPos{ pos };
	const float epsilon{ 0.001f };
	const float bigRadius{ radius + epsilon };
	const float bigRadiusSq{ bigRadius * bigRadius };
	
	while ( SphereCast(unstuckPos, SDL_FPoint{}, radius, walls, Collisions) )
		for ( const auto& c : Collisions )
			if ( c.distSq <= bigRadiusSq )
				unstuckPos += c.normal * (bigRadius - std::sqrt(c.distSq));

	auto CollisionPtrCmp = [](Collision* a, Collision* b)
	{
		return a->t == b->t ? a->distSq < b->distSq : a->t < b->t;
	};
	
	if ( SphereCast(unstuckPos, step, radius, walls, Collisions) )
	{
		CollPtrs.clear();
		for ( auto& c : Collisions ) CollPtrs.push_back(&c);
		Collision* c{ *std::min_element(CollPtrs.begin(), CollPtrs.end(), CollisionPtrCmp) };
		
		SDL_FPoint intermediate = unstuckPos + step * c->t + c->normal * epsilon;
		step = ((1.f - c->t) * dot(c->wallDir, step)) * c->wallDir;

		if ( SphereCast(intermediate, step, radius, walls, Collisions) )
		{
			CollPtrs.clear();
			for ( auto& c : Collisions ) CollPtrs.push_back(&c);
			float t = (*std::min_element(CollPtrs.begin(), CollPtrs.end(), CollisionPtrCmp))->t;
			intermediate += step * t;
		}
		else
		{
			intermediate += step;
		}
		
		return intermediate;
	}

	return unstuckPos + step;
}

bool SphereCast(
	SDL_FPoint castStart,
	SDL_FPoint step,
	float radius,
	const std::vector<Wall>& walls,
	std::vector<Collision>& results)
{
	auto pointCollision = [&castStart](const Wall* w, SDL_FPoint wallEnd, SDL_FPoint p, float t)->Collision
	{
		auto normal = Normalized(p - wallEnd);
		return Collision{ w, wallEnd, LeftFace(normal), normal, t, Norm2(p - castStart) };
	};

	results.clear();
	const float stepLen = Norm(step);
	const SDL_FPoint castNormal = LeftFace(step) / stepLen;
	const float radiusSq = radius * radius;

	for ( const Wall& w : walls )
	{
		const SDL_FPoint end1rel = w.end1-castStart;
		const SDL_FPoint end2rel = w.end2-castStart;

		
		// Check for endpoints from start position in any case
		
		if ( Norm2(end1rel) <= radiusSq )
		{
			results.push_back(pointCollision(&w, w.end1, castStart, 0));
		}
		if ( Norm2(end2rel) <= radiusSq )
		{
			results.push_back(pointCollision(&w, w.end2, castStart, 0));
		}
		

		// Check for the line itself at start pos

		SDL_FPoint wallDir{ w.end2 - w.end1 };
		float wallLen = Norm(wallDir);
		wallDir *= 1 / wallLen;
		SDL_FPoint startRelToWall{ castStart - w.end1 };
		SDL_FPoint wallNormal{ Normalized(PerpTowards(startRelToWall, wallDir)) };
		float startProj = dot(wallNormal, startRelToWall);
		if ( -radius <= startProj && startProj <= radius )
		{
			float wallPortion = dot(wallDir, startRelToWall);
			if ( 0 <= wallPortion && wallPortion <= wallLen )
			{
				SDL_FPoint contact = w.end1 + wallDir * wallPortion;
				results.emplace_back(&w, contact, wallDir, wallNormal, 0, Norm2(castStart - contact));
			}
		}
		if ( stepLen == 0  )
		{
			continue; // With zero step, proceed no further
		}

		
		// If step not zero and therefore castNormal not NaN,
		// finally cast the sphere forwards
		
		const float crossrange1 = dot(end1rel, castNormal);
		const float crossrange2 = dot(end2rel, castNormal);
		if ( crossrange1 > radius && crossrange2 > radius ) continue;
		if ( crossrange1 < -radius && crossrange2 < -radius ) continue;


		// Check for endpoints with non zero step

		if ( -radius <= crossrange1 && crossrange1 <= radius )
		{
			float downRange = dot(end1rel, step) / stepLen;
			if ( (0 <= downRange && downRange <= stepLen) || Norm2(end1rel - step) <= radiusSq )
			{
				float sin = crossrange1 / radius;
				float cos = std::sqrt(1 - sin * sin);
				float t = (downRange - cos * radius) / stepLen;
				auto point = castStart + t * step;
				//auto point = castStart;
				results.push_back(pointCollision(&w, w.end1, point, t));
			}
		}

		if ( -radius <= crossrange2 && crossrange2 <= radius )
		{
			float downRange = dot(end2rel, step) / stepLen;
			if ( (0 <= downRange && downRange <= stepLen) || Norm2(end2rel - step) <= radiusSq )
			{
				float sin = crossrange2 / radius;
				float cos = std::sqrt(1 - sin * sin);
				float t = (downRange - cos * radius) / stepLen;
				auto point = castStart + t * step;
				//auto point = castStart;
				results.push_back(pointCollision(&w, w.end2, point, t));
			}
		}


		// Check for the line itself

		float fullProj = dot(step, wallNormal);
		float t = (radius - startProj) / fullProj;
		if ( 0 <= t && t <= 1 )
		{
			SDL_FPoint center = castStart + t * step;
			float wallPortion = dot(wallDir, center - w.end1);
			if ( 0 <= wallPortion && wallPortion <= wallLen )
			{
				SDL_FPoint contact = w.end1 + wallDir * wallPortion;
				results.emplace_back(&w, contact, wallDir, wallNormal, t, Norm2(castStart - contact));
			}
		}
	}

	return results.size() > 0;
}
