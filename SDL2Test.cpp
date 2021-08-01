#include <iostream>
#include <SDL.h>
#include "SDL2_RAII.h"
#include <vector>
#include "Utils.h"
#include "Controls.h"
#include "Renderings.h"
#include <sstream>


int main(int argc, char* argv[])
{
	constexpr int WIDTH = 1200;
	constexpr int HEIGHT = 400;
	constexpr int SCREENS = 3;

	sdl::Init quitter{ SDL_INIT_VIDEO };
	sdl::Window window{ u8"Collision detection Testing", WIDTH, HEIGHT };
	sdl::Renderer renderer{ window.CreateRenderer() };

	std::vector<Wall> walls;
	walls.push_back(Wall{ 100, 150, 200, 150, SDL_Color{0xff, 0xff, 0x00, 0xff} });
	walls.push_back(Wall{ 200, 150, 200, 100, SDL_Color{0xff, 0x00, 0x00, 0xff} });
	walls.push_back(Wall{ 200, 100, 250, 100, SDL_Color{0x00, 0xff, 0x00, 0xff} });

	Player player{ 150.f, 250.f, PI/2 };
	if ( argc > 1 )
	{
		float inputRadius{ player.radius };
		try
		{
			inputRadius = std::stof(argv[1]);
		}
		catch ( ... )
		{
			std::ostringstream formatter;
			formatter << u8"Couldn't parse command line parameter for player radius.\n\n"
				<< u8"Will use default value of "
				<< player.radius << u8" instead.";

			SDL_ShowSimpleMessageBox(
				SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR,
				u8"Virheilmoitus",
				formatter.str().c_str(),
				window);
		}
		if (inputRadius >= 0 )
			player.radius = inputRadius;
		else
		{
			std::ostringstream formatter;
			formatter << u8"Command line parameter for player radius is silly.\n\n"
				<< u8"Will use default value of "
				<< player.radius << u8" instead.\n\n"
				<< u8"Please use a non negative number next time.";

			SDL_ShowSimpleMessageBox(
				SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR,
				u8"Virheilmoitus",
				formatter.str().c_str(),
				window);
		}
	}


	// The "game loop"
	for ( MoveCommand inputs{}; !inputs.quit; )
	{
		SDL_Delay(10);

		Gather_inputs(&inputs);
		player.Updatepos(inputs,walls);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		// Absolute scene
		SDL_Rect multiUseRect{ 0, 0, WIDTH / SCREENS, HEIGHT };
		SDL_RenderSetViewport(renderer, &multiUseRect);
		RenderWalls2D(renderer, walls);
		RenderPlayer(renderer, player, true);
		

		// Relative scene
		multiUseRect = { WIDTH / SCREENS + 1, 0, WIDTH / SCREENS, HEIGHT };
		SDL_RenderSetViewport(renderer, &multiUseRect);
		auto Tf0 = Rotation(PI/2 - player.dir) * Translation(-player.pos);
		RenderWalls2D(renderer, walls, Tf0);
		RenderPlayer(renderer, player, false);


		// 3D scene
		multiUseRect = { 2 * WIDTH / SCREENS + 1, 0, WIDTH / SCREENS, HEIGHT };
		SDL_RenderSetViewport(renderer, &multiUseRect);
		RenderWalls3D(renderer, walls, Tf0);
		

		// Vertical lines
		SDL_RenderSetViewport(renderer, nullptr);
		SDL_SetRenderDrawColor(renderer, 180, 30, 180, 255);
		for ( int i = 1; i < SCREENS; ++i )
		{
			multiUseRect = { i*WIDTH/SCREENS-1, 0, 3, HEIGHT };
			SDL_RenderFillRect(renderer, &multiUseRect);
		}

		renderer.RenderPresent();
	}
	
	return 0;
}
