#include "Vector2.h"

namespace mlib
{

	Vector2::Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float Vector2::dot(Vector2& v)
	{
		return x * v.x + y * v.y;
	}

	float Vector2::dot(Vector2& a, Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	void Vector2::rotate(float angle)
	{
		float s = sinf(angle);
		float c = cosf(angle);
		float prevX = x;
		x = x * c - y * s;
		y = prevX * s + y * c;
		
	}

	Vector2 Vector2::rotated(float angle)
	{
		float s = sinf(angle);
		float c = cosf(angle);
		return Vector2(x * c - y * s, x * s + y * c);
	}

	float Vector2::magnitude()
	{
		return sqrtf(x * x + y * y);
	}

	float Vector2::magnitudeSquared()
	{
		return x * x + y * y;

	}

	void Vector2::normalize()
	{
		float mag = magnitude();
		if (mag == 0.0f)
			return;
		x = x / mag;
		y = y / mag;
	}

	Vector2 Vector2::normalised()
	{
		float mag = magnitude();
		if (mag == 0.0f)
			return { 0 };
		return Vector2(x / mag, y / mag);
	}

	Vector2 Vector2::perpendicular()
	{
		return Vector2(-y, x);
	}

	float Vector2::getAngle(Vector2& a, Vector2& b)
	{
		Vector2 norm = a.normalised();

		Vector2 rightAngle = norm.perpendicular();
		float aDot = norm.dot(b);
		float pDot = rightAngle.dot(b);
		float angle = acosf(aDot);
		if (pDot > 0)
			angle *= -1;
		return angle;
	}

	float Vector2::getAngle(Vector2& v)
	{
		Vector2 norm = normalised();

		Vector2 rightAngle = norm.perpendicular();
		float aDot = norm.dot(v);
		float pDot = rightAngle.dot(v);
		float angle = acosf(aDot);
		if (pDot > 0)
			angle *= -1;
		return angle;
	}

	Vector2 Vector2::lerp(Vector2& a, Vector2& b, float t)
	{
		return Vector2(mlib::lerp(a.x, b.x, t), mlib::lerp(a.y, b.y, t));
	}

	Vector2 Vector2::operator+ (Vector2 const& v)
	{
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 Vector2::operator- (Vector2 const& v)
	{
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 Vector2::operator* (Vector2 const& v)
	{
		return Vector2(x * v.x, y * v.y);
	}
	Vector2 Vector2::operator* (float f)
	{
		return Vector2(x * f, y * f);
	}
	Vector2 Vector2::operator/ (float f)
	{
		return Vector2(x / f, y / f);
	}
	Vector2 Vector2::operator/ (Vector2 const& v)
	{
		return Vector2(x / v.x, y / v.y);
	}
	Vector2::operator RLVector2()
	{
		return RLVector2{ x,y };
	}
	Vector2::operator b2Vec2()
	{
		return b2Vec2{ x,y };
	}
}