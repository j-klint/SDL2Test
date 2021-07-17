#pragma once
#include <SDL.h>
#include <vector>
#include "SDL2_RAII.h"
#include "Controls.h"
#include "Matrix.h"

struct Wall
{
	SDL_FPoint end1, end2;
	SDL_Color color;
	Wall(float x1, float y1, float x2, float y2, SDL_Color clr)
		:end1{ x1,y1 }, end2{ x2,y2 }, color{ clr } {}
};

enum struct IntersectType
{
	first_out, second_out, both_in, both_out
};

IntersectType TestForIntersection(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& dir);

/// Don't use this function unless you've checked that there indeed is an intersection.
SDL_FPoint IntersectionPoint(const SDL_FPoint& end1, const SDL_FPoint& end2, const SDL_FPoint& dir);

void RenderWalls2D(sdl::Renderer& renderer, const std::vector<Wall>& walls);

void RenderWalls2D(
	sdl::Renderer& renderer,
	const std::vector<Wall>& walls,
	const Matrix<3, 3, float>& ViewTransform);

void RenderWalls3D(
	sdl::Renderer& renderer,
	const std::vector<Wall>& walls,
	const Matrix<3, 3, float>& ViewTransform);

void RenderPlayer(SDL_Renderer* renderer, const Player& player, bool absolute);
