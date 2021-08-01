#include "Utils.h"

SDL_FPoint& operator*=(SDL_FPoint& p, float f)
{
	p.x *= f;
	p.y *= f;
	return p;
}

SDL_FPoint& operator*=(float f, SDL_FPoint& p)
{
	p.x *= f;
	p.y *= f;
	return p;
}

SDL_FPoint& operator-=(SDL_FPoint& p1, const SDL_FPoint& p2)
{
	p1.x -= p2.x;
	p1.y -= p2.y;
	return p1;
}

SDL_FPoint& operator+=(SDL_FPoint& p1, const SDL_FPoint& p2)
{
	p1.x += p2.x;
	p1.y += p2.y;
	return p1;
}

Matrix<3, 3, float> Rotation(float angle)
{
	return Matrix<3, 3, float>{
		{  std::cos(angle), std::sin(angle), 0 },
		{ -std::sin(angle), std::cos(angle), 0 },
		{                0,               0, 1 }
	};
}
