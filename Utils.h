#pragma once
#include <SDL.h>
#include "Matrix.h"

constexpr float PI = static_cast<float>(M_PI);

constexpr float dot(const SDL_FPoint& a, const SDL_FPoint& b) { return a.x * b.x + a.y * b.y; }
constexpr SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) { return { a.x + b.x, a.y + b.y }; }
constexpr SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) { return { a.x - b.x, a.y - b.y }; }
constexpr SDL_FPoint operator-(const SDL_FPoint& p) { return { -p.x, -p.y }; }
SDL_FPoint& operator*=(SDL_FPoint& p, float f);
SDL_FPoint& operator-=(SDL_FPoint& p1, const SDL_FPoint& p2);
SDL_FPoint& operator+=(SDL_FPoint& p1, const SDL_FPoint& p2);
float Norm(const SDL_FPoint& p);
void Normalize(SDL_FPoint& p);
SDL_FPoint Normalized(const SDL_FPoint& p);
constexpr float Norm2(const SDL_FPoint& p) { return dot(p, p); }
constexpr SDL_FPoint operator*(float f, const SDL_FPoint& p) { return { p.x * f, p.y * f }; }
constexpr SDL_FPoint operator*(const SDL_FPoint& p, float f) { return { p.x * f, p.y * f }; }
constexpr SDL_FPoint operator/(const SDL_FPoint& p, float f) { return (1.f / f) * p; }

constexpr SDL_FPoint PerpTowards(SDL_FPoint point, SDL_FPoint fromVec)
{
	float X{ point.y * fromVec.x - point.x * fromVec.y };
	return { -X * fromVec.y, X * fromVec.x };
}

constexpr Matrix<3, 3, float> Translation(const SDL_FPoint& d)
{
	return Matrix<3, 3, float>{
		{ 1.f, 0.f, d.x },
		{ 0.f, 1.f, d.y },
		{ 0.f, 0.f, 1.f }
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

constexpr SDL_FPoint LeftFace(SDL_FPoint p) { return { p.y, -p.x }; };
constexpr SDL_FPoint RightFace(SDL_FPoint p) { return { -p.y, p.x }; };

constexpr SDL_FPoint operator*(const Matrix<3, 3, float>& Transform, const SDL_FPoint pt)
{
	const auto temp = Transform * Matrix<3, 1, float>{ pt.x, pt.y, 1.f };
	return { temp[0][0], temp[1][0] };
}


enum struct IntersectType
{
	first_out, second_out, both_in, both_out
};

IntersectType TestForIntersection(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& dir);

/// Don't use this function unless you've checked that there indeed is an intersection.
SDL_FPoint IntersectionPoint(const SDL_FPoint& end1, const SDL_FPoint& end2, const SDL_FPoint& dir);
