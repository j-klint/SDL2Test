#include "Renderings.h"
#include "Utils.h"


void RenderWalls2D(sdl::Renderer& renderer, const std::vector<Wall>& walls)
{
	for ( const auto& w : walls )
	{
		renderer.SetRenderDrawColor(w.color);
		renderer.RenderDrawLineF(w.end1, w.end2);
	}
}

void RenderWalls2D(
	sdl::Renderer& renderer,
	const std::vector<Wall>& walls,
	const Matrix<3, 3, float>& ViewTransform)
{
	SDL_Rect frame;
	SDL_RenderGetViewport(renderer, &frame);
	auto Tr = Translation(frame.w/2, frame.h/2) * ViewTransform;
	
	for ( const auto& w : walls )
	{
		renderer.SetRenderDrawColor(w.color);
		renderer.RenderDrawLineF(Tr(w.end1), Tr(w.end2));
	}
}

void RenderWalls3D(
	sdl::Renderer& renderer,
	const std::vector<Wall>& walls,
	const Matrix<3, 3, float>& ViewTransform)
{
	SDL_FPoint center;
	SDL_Rect frame;
	SDL_RenderGetViewport(renderer, &frame);
	center.x = frame.w / 2.f;
	center.y = frame.h / 2.f;

	auto proj = [&center, width = frame.w / 2](SDL_FPoint p, float h)->SDL_FPoint
	{
		float c = -1 / p.y * width;
		p = { p.x * c, h * c };
		return p + center;
	};

	for ( const auto& w : walls )
	{
		SDL_FPoint e[2]{ ViewTransform(w.end1), ViewTransform(w.end2) };

		SDL_FPoint testvec{ -1, -1 };
		switch ( TestForIntersection(e[0], e[1], testvec) )
		{
		case IntersectType::both_out:
			continue; break;
		case IntersectType::first_out:
			e[0] = IntersectionPoint(e[0], e[1], testvec); break;
		case IntersectType::second_out:
			e[1] = IntersectionPoint(e[0], e[1], testvec); break;
		}

		testvec = { -testvec.x, testvec.y };
		switch ( TestForIntersection(e[0], e[1], testvec) )
		{
		case IntersectType::both_out:
			continue; break;
		case IntersectType::first_out:
			e[0] = IntersectionPoint(e[0], e[1], testvec); break;
		case IntersectType::second_out:
			e[1] = IntersectionPoint(e[0], e[1], testvec); break;
		}

		renderer.SetRenderDrawColor(w.color);
		SDL_FPoint wireframe[5];
		wireframe[0] = proj(e[0], -10.f);
		wireframe[1] = proj(e[0], 20.f);
		wireframe[2] = proj(e[1], 20.f);
		wireframe[3] = proj(e[1], -10.f);
		wireframe[4] = wireframe[0];
		SDL_RenderDrawLinesF(renderer, wireframe, 5);
	}
}
	
void RenderPlayer(SDL_Renderer* renderer, const Player& player, bool absolute)
{
	SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);

	SDL_FPoint pos;
	float dir;
	if ( absolute )
	{
		pos = player.pos;
		dir = player.dir;
	}
	else
	{
		SDL_Rect frame;
		SDL_RenderGetViewport(renderer, &frame);
		pos.x = frame.w / 2.f;
		pos.y = frame.h / 2.f;
		dir = PI/2;
	}

	SDL_RenderDrawLine(
		renderer,
		pos.x, pos.y,
		pos.x + std::cos(dir) * player.NoseLength,
		pos.y - std::sin(dir) * player.NoseLength
	);

	SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
	SDL_Rect plrrect{ pos.x - 1, pos.y - 1, 3, 3 };
	SDL_RenderFillRect(renderer, &plrrect);
}
