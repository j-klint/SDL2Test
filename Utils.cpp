#include "Utils.h"

SDL_FPoint& operator*=(SDL_FPoint& p, float f)
{
	p.x *= f;
	p.y *= f;
	return p;
}

Matrix<3, 3, float> Rotation(float angle)
{
	return Matrix<3, 3, float>{
		{  std::cos(angle), std::sin(angle), 0 },
		{ -std::sin(angle), std::cos(angle), 0 },
		{ 0,              0                , 1 }
	};
}
