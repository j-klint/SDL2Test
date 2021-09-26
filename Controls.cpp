#include <cmath>
#include <algorithm>
#include "Controls.h"
#include "Renderings.h"
#include "Utils.h"

static std::vector<Collision> Collisions(6);

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

void Player::Updatepos(const MoveCommand& cmd, const std::vector<Wall>& walls, float speed, float turnRate)
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
	static constexpr float epsilon{ 0.0001f };
	const float bigRadius{ radius + epsilon };
	const float bigRadiusSq{ bigRadius * bigRadius };
	
	while ( SphereCast(unstuckPos, SDL_FPoint{}, radius, walls, Collisions) )
		for ( const auto& c : Collisions )
			if ( c.distSq < bigRadiusSq )
				unstuckPos += c.normal * (bigRadius - std::sqrt(c.distSq));

	if ( SphereCast(unstuckPos, step, radius, walls, Collisions) )
	{
		auto CollisionCmp = [](const Collision& a, const Collision& b) -> bool
		{
			return a.t == b.t ? a.distSq < b.distSq : a.t < b.t;
		};
		
		const auto c = std::min_element(Collisions.begin(), Collisions.end(), CollisionCmp);
		SDL_FPoint intermediate{ unstuckPos + step * c->t + c->normal * epsilon };
		step = ((1.f - c->t) * dot(c->wallDir, step)) * c->wallDir;

		if ( SphereCast(intermediate, step, radius, walls, Collisions) )
		{
			const float t{ std::min_element(Collisions.begin(), Collisions.end(), CollisionCmp)->t };
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
	const SDL_FPoint castStart,
	const SDL_FPoint step,
	const float radius,
	const std::vector<Wall>& walls,
	std::vector<Collision>& results)
{
	auto pointCollision = [&castStart](const Wall* w, SDL_FPoint wallEnd, SDL_FPoint p, float t)->Collision
	{
		SDL_FPoint normal{ Normalized(p - wallEnd) };
		return Collision{ w, wallEnd, LeftFace(normal), normal, t, Norm2(p - castStart) };
	};

	results.clear();
	const float stepLen = Norm(step);
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

		const SDL_FPoint castStartRelToWall{ castStart - w.end1 };
		SDL_FPoint wallDir{ w.end2 - w.end1 };
		const SDL_FPoint wallNormal{ Normalized(PerpTowards(castStartRelToWall, wallDir)) };
		float wallLen = Norm(wallDir);
		wallDir *= 1 / wallLen;
		float startProj = dot(wallNormal, castStartRelToWall);
		if ( -radius <= startProj && startProj <= radius )
		{
			float wallPortion = dot(wallDir, castStartRelToWall);
			if ( 0 <= wallPortion && wallPortion <= wallLen )
			{
				SDL_FPoint contact = w.end1 + wallDir * wallPortion;
				results.emplace_back(&w, contact, wallDir, wallNormal, 0.f, Norm2(castStart - contact));
			}
		}
		if ( stepLen == 0  )
		{
			continue; // With zero step, proceed no further
		}

		

		// If step not zero and therefore 1/stepLen not NaN,
		// finally cast the sphere forwards
		
		const SDL_FPoint castNormal = LeftFace(step) / stepLen;
		const float crossrange[2]{ dot(end1rel, castNormal), dot(end2rel, castNormal) };
		if ( crossrange[0] > radius  && crossrange[1] > radius ) continue;
		if ( crossrange[0] < -radius && crossrange[1] < -radius ) continue;


		// Check for endpoints with non zero step
		
		const SDL_FPoint relEnds[2]{ end1rel, end2rel };
		for ( int i = 0; i < 2; ++i )
		{
			if ( -radius <= crossrange[i] && crossrange[i] <= radius )
			{
				const float downRange = dot(relEnds[i], step) / stepLen;
				if ( (0 <= downRange && downRange <= stepLen) || Norm2(relEnds[i] - step) <= radiusSq )
				{
					const float sin = crossrange[i] / radius;
					const float cos = std::sqrt(1 - sin * sin);
					const float t = (downRange - cos * radius) / stepLen;
					const auto point = castStart + t * step;
					results.push_back(pointCollision(&w, w.end1, point, t));
				}
			}
		}
		

		// Check for the line itself

		const float fullProj = dot(step, wallNormal);
		const float t = (radius - startProj) / fullProj;
		if ( 0 <= t && t <= 1 )
		{
			const SDL_FPoint center = castStart + t * step;
			const float wallPortion = dot(wallDir, center - w.end1);
			if ( 0 <= wallPortion && wallPortion <= wallLen )
			{
				const SDL_FPoint contact = w.end1 + wallDir * wallPortion;
				results.emplace_back(&w, contact, wallDir, wallNormal, t, Norm2(castStart - contact));
			}
		}
	}

	return results.size() > 0;
}
