#include <iostream>
#include <SDL.h>
#include "SDL2_RAII.h"
#include <vector>
#include "Matrix.h"
#include "Controls.h"

constexpr float PI(M_PI);

struct Wall
{
	SDL_FPoint end1, end2;
	SDL_Color color;
	Wall(float x1, float y1, float x2, float y2, SDL_Color clr)
		:end1{ x1,y1 }, end2{ x2,y2 }, color{ clr } {}
};


// Nää constexprit ei toimi, jos ne määrittelee vasta myöhemmin.
constexpr Matrix<3, 3, float> Translation(const SDL_FPoint& d)
{
	return Matrix<3, 3, float>{
		{ 1, 0, d.x },
		{ 0, 1, d.y },
		{ 0, 0,   1 }
	};
}

// Pyh.Ei ole constexpr trigonometrisiä funktioita.
Matrix<3, 3, float> Rotation(float angle)
{
	return Matrix<3, 3, float>{
		{  std::cos(angle), std::sin(angle), 0 },
		{ -std::sin(angle), std::cos(angle), 0 },
		{                0,               0, 1 }
	};
}

constexpr Matrix<3, 3, float> LeftFace{
		{  0, 1, 0 },
		{ -1, 0, 0 },
		{  0, 0, 1 }
};
constexpr Matrix<3, 3, float> RightFace{
		{ 0, -1, 0 },
		{ 1,  0, 0 },
		{ 0,  0, 1 }
};

// cf. Matrix<3,3,float>::operator()(const SDL_FPoint&)
/*SDL_FPoint operator*(const Matrix<3, 3, float>& M, const SDL_FPoint& pt)
{
	auto temp = M * Matrix<3, 1, float>{pt.x, pt.y, 1};
	return { temp[0][0],temp[1][0] };
}*/

enum struct IntersectType
{
	first_out, second_out, both_in, both_out
};

IntersectType TestForIntersection(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& dir);

/// Don't use this unless you've checked that there indeed is an intersection.
SDL_FPoint IntersectionPoint(const SDL_FPoint& end1, const SDL_FPoint& end2, const SDL_FPoint& dir);

constexpr float dot(const SDL_FPoint& a, const SDL_FPoint& b) { return a.x*b.x + a.y*b.y; }

int main(int argc, char* argv[])
{
	constexpr int WIDTH = 1200;
	constexpr int HEIGHT = 400;
	constexpr int SCREENS = 3;
	constexpr int NOSELENGTH = 10;

	sdl::Init quitter{ SDL_INIT_VIDEO };
	sdl::Window window{ u8"SDL2 Côördïnàte Tränsform Testiñg", WIDTH, HEIGHT };
	sdl::Renderer renderer{ window.CreateRenderer() };

	std::vector<Wall> walls;
	walls.emplace_back(100, 150, 200, 150, SDL_Color{0xff, 0xff, 0x00, 0xff});
	walls.emplace_back(200, 150, 200, 100, SDL_Color{0xff, 0x00, 0x00, 0xff});
	walls.emplace_back(200, 100, 250, 100, SDL_Color{0x00, 0xff, 0x00, 0xff});

	Player player{ 150,250, PI/2 };
	constexpr SDL_FPoint origo { WIDTH / SCREENS * 1.5, HEIGHT / 2 };
	constexpr SDL_FPoint origo2{ WIDTH / SCREENS * 2.5, HEIGHT / 2 };
	constexpr auto PlacementTf = Translation(origo);
	
	for ( MoveCommand inputs{}; !inputs.quit; )
	{
		SDL_Delay(10);
		Gather_inputs(&inputs);
		player.Updatepos(inputs);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);



		// Absolute scene
		SDL_Rect clippin{ 0, 0, WIDTH / SCREENS, HEIGHT };
		SDL_RenderSetClipRect(renderer, &clippin);

		for ( const auto& w : walls )
		{
			renderer.SetRenderDrawColor(w.color);
			renderer.RenderDrawLineF(w.end1, w.end2);
		}

		SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
		SDL_RenderDrawLineF(
			renderer,
			player.pos.x, player.pos.y,
			player.pos.x + std::cos(player.dir) * NOSELENGTH,
			player.pos.y - std::sin(player.dir) * NOSELENGTH
		);
		SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
		SDL_FRect plrrect{ player.pos.x - 1, player.pos.y - 1, 3, 3 };
		SDL_RenderFillRectF(renderer, &plrrect);

		

		// Relative scene
		clippin={ WIDTH/ SCREENS + 1, 0, WIDTH/ SCREENS, HEIGHT };
		SDL_RenderSetClipRect(renderer, &clippin);

		auto Tf0 = Rotation(PI/2 - player.dir) * Translation(-player.pos);
		auto Tf = PlacementTf * Tf0;
		for ( const auto& w : walls )
		{
			renderer.SetRenderDrawColor(w.color);
			renderer.RenderDrawLineF(Tf(w.end1), Tf(w.end2));
		}

		SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
		SDL_RenderDrawLineF(
			renderer,
			origo.x, origo.y,
			origo.x,
			origo.y - NOSELENGTH
		);
		SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
		plrrect={ origo.x - 1, origo.y - 1, 3, 3 };
		SDL_RenderFillRectF(renderer, &plrrect);



		// 3D scene
		auto proj = [&origo2, width = WIDTH/SCREENS/2](SDL_FPoint p, float h) -> SDL_FPoint
		{
			float c = -1 / p.y * width;
			p = { p.x * c, h * c };
			return p + origo2;
		};
		
		clippin = { 2 * WIDTH / SCREENS + 1, 0, WIDTH / SCREENS, HEIGHT };
		SDL_RenderSetClipRect(renderer, &clippin);

		for ( const auto& w : walls )
		{
			SDL_FPoint e[2]{ Tf0(w.end1), Tf0(w.end2) };
			
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



		// Lopuksi Pystyviivat
		clippin = { 0,0,WIDTH,HEIGHT };
		SDL_RenderSetClipRect(renderer, &clippin);
		SDL_SetRenderDrawColor(renderer, 180, 30, 180, 255);
		for ( int i = 1; i < SCREENS; ++i )
		{
			clippin = { i*WIDTH/SCREENS-1, 0, 3, HEIGHT };
			SDL_RenderFillRect(renderer, &clippin);
		}

		renderer.RenderPresent();
	}

	std::cout << "Quit event encountered\n";
	return 0;
}

IntersectType TestForIntersection(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& dir)
{
	SDL_FPoint normal{ dir.x < 0 ? RightFace(dir) : LeftFace(dir) };
	float dot1 = dot(normal, a);
	float dot2 = dot(normal, b);

	if ( dot1 < 0 && dot2 > 0 ) return IntersectType::first_out;
	if ( dot1 > 0 && dot2 < 0 ) return IntersectType::second_out;
	if ( dot1 > 0 && dot2 > 0 ) return IntersectType::both_in;
	return IntersectType::both_out;
}

SDL_FPoint IntersectionPoint(const SDL_FPoint& end1, const SDL_FPoint& end2, const SDL_FPoint& dir)
{
	SDL_FPoint r = end2 - end1;
	float d = dir.y / dir.x;
	float s = (end1.y - end1.x * d) / (d * r.x - r.y);
	return s * r + end1;
}
