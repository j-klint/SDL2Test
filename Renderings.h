#pragma once
#include <SDL.h>
#include <vector>
#include "SDL2_RAII.h"
#include "Controls.h"
#include "Matrix.h"



void RenderWalls2D(sdl::Renderer& renderer, const std::vector<Wall>& walls);

void RenderWalls2D(
	sdl::Renderer& renderer,
	const std::vector<Wall>& walls,
	const Matrix<3, 3, float>& ViewTransform
);

void RenderWalls3D(
	sdl::Renderer& renderer,
	const std::vector<Wall>& walls,
	const Matrix<3, 3, float>& ViewTransform
);

void RenderPlayer(SDL_Renderer* renderer, const Player& player, bool absolute);
