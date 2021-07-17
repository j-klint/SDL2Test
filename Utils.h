#pragma once
#include "Matrix.h"

constexpr float dot(const SDL_FPoint& a, const SDL_FPoint& b) { return a.x * b.x + a.y * b.y; }
inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) { return { a.x + b.x, a.y + b.y }; }
inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) { return { a.x - b.x, a.y - b.y }; }
inline SDL_FPoint operator-(const SDL_FPoint& p) { return { -p.x, -p.y }; }
inline SDL_FPoint& operator*=(SDL_FPoint& p, float f)
{
	p.x *= f;
	p.y *= f;
	return p;
}
inline SDL_FPoint operator*(float f, const SDL_FPoint& p)
{
	return { p.x * f, p.y * f };
}

//inline
constexpr Matrix<3, 3, float> Translation(const SDL_FPoint& d)
{
	return Matrix<3, 3, float>{
		{ 1, 0, d.x },
		{ 0, 1, d.y },
		{ 0, 0,   1 }
	};
}

Matrix<3, 3, float> Rotation(float angle);


constexpr float PI = static_cast<float>(M_PI);
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
