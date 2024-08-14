#include "Utils.h"

SDL_FPoint& operator*=(SDL_FPoint& p, float f)
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

float Norm(const SDL_FPoint& p)
{
	return std::sqrt(dot(p, p));
}

void Normalize(SDL_FPoint& p) { p *= 1.f / Norm(p); }

SDL_FPoint Normalized(const SDL_FPoint& p)
{
	return (1.f / Norm(p)) * p;
}

Matrix<3, 3, float> Rotation(float angle)
{
	return Matrix<3, 3, float>{
		{  std::cos(angle), std::sin(angle), 0.f },
		{ -std::sin(angle), std::cos(angle), 0.f },
		{              0.f,             0.f, 1.f }
	};
}

SDL_FPoint operator*(const Matrix<3, 3, float>& Transform, const SDL_FPoint& pt)
{
	auto temp = Transform * Matrix<3, 1, float>{pt.x, pt.y, 1};
	return { static_cast<float>(temp[0][0]), static_cast<float>(temp[1][0]) };
}

IntersectType TestForIntersection(const SDL_FPoint& a, const SDL_FPoint& b, const SDL_FPoint& dir)
{
	SDL_FPoint normal{ dir.x < 0.f ? RightFace(dir) : LeftFace(dir) };
	float dot1 = dot(normal, a);
	float dot2 = dot(normal, b);

	if ( dot1 < 0.f && dot2 > 0.f ) return IntersectType::first_out;
	if ( dot1 > 0.f && dot2 < 0.f ) return IntersectType::second_out;
	if ( dot1 > 0.f && dot2 > 0.f ) return IntersectType::both_in;
	return IntersectType::both_out;
}

SDL_FPoint IntersectionPoint(const SDL_FPoint& end1, const SDL_FPoint& end2, const SDL_FPoint& dir)
{
	SDL_FPoint r = end2 - end1;
	float d = dir.y / dir.x;
	float s = (end1.y - end1.x * d) / (d * r.x - r.y);
	return s * r + end1;
}
