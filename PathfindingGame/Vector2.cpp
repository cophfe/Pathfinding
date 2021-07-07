#include "Vector2.h"

namespace mlib
{

	M_Vector2::M_Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float M_Vector2::dot(M_Vector2* v)
	{
		return x * v->x + y * v->y;
	}

	float M_Vector2::dot(M_Vector2* a, M_Vector2* b)
	{
		return a->x * b->x + a->y * b->y;
	}

	void M_Vector2::rotate(float angle)
	{

	}

	M_Vector2 M_Vector2::rotated(float angle)
	{

	}

	float M_Vector2::magnitude()
	{

	}

	float M_Vector2::magnitudeSquared()
	{

	}

	void M_Vector2::normalize()
	{

	}

	M_Vector2 M_Vector2::normalised()
	{

	}

	M_Vector2 M_Vector2::perpendicular()
	{

	}

	float M_Vector2::getAngle(M_Vector2& a, M_Vector2& b)
	{

	}

	float M_Vector2::getAngle(M_Vector2& v)
	{

	}

	M_Vector2 M_Vector2::lerp(M_Vector2& a, M_Vector2& b, float t)
	{
		return M_Vector2(mlib::lerp(a.x, b.x, t), mlib::lerp(a.y, b.y, t));
	}

	M_Vector2 M_Vector2::operator+ (M_Vector2 const& v)
	{
		return M_Vector2(x + v.x, y + v.y);
	}
	M_Vector2 M_Vector2::operator- (M_Vector2 const& v)
	{
		return M_Vector2(x - v.x, y - v.y);
	}
	M_Vector2 M_Vector2::operator* (M_Vector2 const& v)
	{
		return M_Vector2(x * v.x, y * v.y);
	}
	M_Vector2 M_Vector2::operator* (float f)
	{
		return M_Vector2(x * f, y * f);
	}
	M_Vector2 M_Vector2::operator/ (float f)
	{
		return M_Vector2(x / f, y / f);
	}
	M_Vector2 M_Vector2::operator/ (M_Vector2 const& v)
	{
		return M_Vector2(x / v.x, y / v.y);
	}
}