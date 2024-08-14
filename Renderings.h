#pragma once
#include <vector>

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif //  _WIN32

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
