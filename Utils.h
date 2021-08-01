#pragma once
#include "Matrix.h"

constexpr float dot(const SDL_FPoint& a, const SDL_FPoint& b) { return a.x * b.x + a.y * b.y; }
constexpr SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) { return { a.x + b.x, a.y + b.y }; }
constexpr SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) { return { a.x - b.x, a.y - b.y }; }
constexpr SDL_FPoint operator-(const SDL_FPoint& p) { return { -p.x, -p.y }; }
SDL_FPoint& operator*=(SDL_FPoint& p, float f);
SDL_FPoint& operator*=(float f, SDL_FPoint& p);
SDL_FPoint& operator-=(SDL_FPoint& p1, const SDL_FPoint& p2);
SDL_FPoint& operator+=(SDL_FPoint& p1, const SDL_FPoint& p2);

constexpr SDL_FPoint operator*(float f, const SDL_FPoint& p)
{
	return { p.x * f, p.y * f };
}

constexpr SDL_FPoint operator*(const SDL_FPoint& p, float f)
{
	return { p.x * f, p.y * f };
}

constexpr Matrix<3, 3, float> Translation(const SDL_FPoint& d)
{
	return Matrix<3, 3, float>{
		{ 1, 0, d.x },
		{ 0, 1, d.y },
		{ 0, 0,   1 }
	};
}

constexpr Matrix<3, 3, float> Translation(int dx, int dy)
{
	return Matrix<3, 3, float>{
		{ 1.f, 0.f, static_cast<float>(dx) },
		{ 0.f, 1.f, static_cast<float>(dy) },
		{ 0.f, 0.f, 1.f }
	};
}

Matrix<3, 3, float> Rotation(float angle);

constexpr float PI = static_cast<float>(M_PI);

/*constexpr Matrix<3, 3, float> LeftFace{
		{  0, 1, 0 },
		{ -1, 0, 0 },
		{  0, 0, 1 }
};

constexpr Matrix<3, 3, float> RightFace{
		{ 0, -1, 0 },
		{ 1,  0, 0 },
		{ 0,  0, 1 }
};*/

constexpr SDL_FPoint LeftFace(SDL_FPoint p)
{
		return { p.y, -p.x };
};

constexpr SDL_FPoint RightFace(SDL_FPoint p)
{
	return { -p.y, p.x };
};
