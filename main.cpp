#include <iostream>
#include <vector>
#include <sstream>
#include <SDL.h>

#include "SDL2_RAII.h"
#include "Utils.h"
#include "Controls.h"
#include "Renderings.h"


void ParseCommandLine(Player& player, const char* argv1, SDL_Window* window = nullptr);

int main(int argc, char* argv[])
{
#ifndef NDEBUG
	std::clog << "main started\n";
#endif

	constexpr int WIDTH = 1500;
	constexpr int HEIGHT = WIDTH/3;
	constexpr int SCREENS = 3;
	constexpr SDL_Point OFFSET{ WIDTH/6 - 190, HEIGHT/2 - 250 };

	sdl::Init quitter{ SDL_INIT_VIDEO };
	sdl::Window window{ u8"Collision detection Testing", WIDTH, HEIGHT };
	sdl::Renderer renderer{ window.CreateRenderer() };

	std::vector<Wall> walls;
	walls.emplace_back( OFFSET.x + 100.f, OFFSET.y + 150.f, OFFSET.x + 200.f, OFFSET.y + 150.f, SDL_Color{0xff, 0xff, 0x00, 0xff} );
	walls.emplace_back( OFFSET.x + 200.f, OFFSET.y + 150.f, OFFSET.x + 200.f, OFFSET.y + 100.f, SDL_Color{0xff, 0x00, 0x00, 0xff} );
	walls.emplace_back( OFFSET.x + 200.f, OFFSET.y + 100.f, OFFSET.x + 250.f, OFFSET.y + 100.f, SDL_Color{0x00, 0xff, 0x00, 0xff} );
	walls.emplace_back( OFFSET.x + 250.f, OFFSET.y + 150.f, OFFSET.x + 290.f, OFFSET.y + 110.f, SDL_Color{0xff, 0x70, 0x00, 0xff} );
	walls.emplace_back( OFFSET.x + 250.f, OFFSET.y + 100.f, OFFSET.x + 250.f, OFFSET.y + 150.f, SDL_Color{0x30, 0x30, 0xff, 0xff} );

	Player player{ OFFSET.x + 205.f, OFFSET.y + 250.f, PI/2 };
	if ( argc > 1 )
	{
		ParseCommandLine(player, argv[1]);
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
	
#ifndef NDEBUG
	std::clog << "End\n";
#endif
	return 0;
}


void ParseCommandLine(Player& player, const char* argv1, SDL_Window* window)
{
	float inputRadius{ player.radius };
	try
	{
		inputRadius = std::stof(argv1);
	}
	catch ( ... )
	{
		std::ostringstream formatter;
		formatter << u8"Couldn't parse command line parameter for player radius.\n\n"
			<< u8"Will use default value of "
			<< player.radius << u8" instead.";

		SDL_ShowSimpleMessageBox(
			SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING,
			u8"Virheilmoitus",
			formatter.str().c_str(),
			window);
	}

	if ( inputRadius >= 0 )
		player.radius = inputRadius;
	else
	{
		std::ostringstream formatter;
		formatter << u8"Command line parameter for player radius is silly.\n\n"
			<< u8"Will use default value of "
			<< player.radius << u8" instead.\n\n"
			<< u8"Please use a non negative number next time.";

		SDL_ShowSimpleMessageBox(
			SDL_MessageBoxFlags::SDL_MESSAGEBOX_WARNING,
			u8"Virheilmoitus",
			formatter.str().c_str(),
			window);
	}
}
