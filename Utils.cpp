#include "Utils.h"

//extern float PI;

//constexpr
//Matrix<3, 3, float> Translation(const SDL_FPoint& d);
/* {
	return Matrix<3, 3, float>{
		{ 1, 0, d.x },
		{ 0, 1, d.y },
		{ 0, 0,   1 }
	};
}*/

Matrix<3, 3, float> Rotation(float angle)
{
	return Matrix<3, 3, float>{
		{  std::cos(angle), std::sin(angle), 0 },
		{ -std::sin(angle), std::cos(angle), 0 },
		{ 0,               0, 1 }
	};
}

//extern constexpr Matrix<3, 3, float> LeftFace;
//extern constexpr Matrix<3, 3, float> RightFace;
