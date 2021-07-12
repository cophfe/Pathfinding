#include "Matrix3.h"

namespace mlib
{

	Matrix3::Matrix3(float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8)
	{
		m[0] = m0;
		m[1] = m1;
		m[2] = m2;
		m[3] = m3;
		m[4] = m4;
		m[5] = m5;
		m[6] = m6;
		m[7] = m7;
		m[8] = m8;
	}

	Matrix3 Matrix3::inverse()
	{
		float d0 = m[4] * m[8] - m[5] * m[7];
		float d3 = m[1] * m[8] - m[7] * m[2];
		float d6 = m[1] * m[5] - m[4] * m[2];
		float det = m[0] * d0 - m[3] * d3 + m[6] * d6;

		Matrix3 i = Matrix3();
		if (det == 0)
			return i;

		float iDet = 1 / det;
		i.m[0] = d0 * iDet;
		i.m[1] = -d3 * iDet;
		i.m[2] = d6 * iDet;
		i.m[3] = -(m[3] * m[8] - m[6] * m[5]) * iDet;
		i.m[4] = (m[0] * m[8] - m[6] * m[2]) * iDet;
		i.m[5] = -(m[0] * m[5] - m[3] * m[2]) * iDet;
		i.m[6] = (m[3] * m[7] - m[6] * m[4]) * iDet;
		i.m[7] = -(m[0] * m[7] - m[6] * m[1]) * iDet;
		i.m[8] = (m[0] * m[4] - m[0] * m[1]) * iDet;

		return i;
	}

	Vector2 Matrix3::getScale()
	{
		return Vector2(sqrtf(m[0] * m[0] + m[1] * m[1]), sqrtf(m[3] * m[3] + m[4] * m[4]));
	}

	float Matrix3::getScaleX()
	{
		return sqrtf(m[0] * m[0] + m[1] * m[1]);
	}

	Vector2 Matrix3::getTranslation()
	{
		return Vector2(m[6], m[7]);
	}

	float Matrix3::getZRotation()
	{
		float scaleX = sqrtf(m[0] * m[0] + m[1] * m[1]);
		return atan2(m[1] / scaleX, m[0] / scaleX);
	}

	void Matrix3::getAllTransformations(Vector2* position, Vector2* scale, float* rotation)
	{
		*position = Vector2(m[6], m[7]);
		*scale = Vector2(sqrtf(m[0] * m[0] + m[1] * m[1]), sqrtf(m[3] * m[3] + m[4] * m[4]));
		*rotation = atan2(m[1], m[0]);
	}

	void Matrix3::getAllTransformations(Vector2* position, float* scale, float* rotation)
	{
		*position = Vector2(m[6], m[7]);
		*scale = sqrtf(m[0] * m[0] + m[1] * m[1]);
		*rotation = atan2(m[1], m[0]);
	}

	Vector2 Matrix3::getUpVector()
	{
		return Vector2(m[3], m[4]);
	}

	Vector2 Matrix3::getRightVector()
	{
		return Vector2(m[0], m[1]);
	}

	Matrix3 Matrix3::getPositionMatrix(Vector2& position)
	{
		return Matrix3(1, 0, 0, 0, 1, 0, position.x, position.y, 1);
	}

	Matrix3 Matrix3::getRotationMatrix2D(float angle)
	{
		float sin = sinf(angle);
		float cos = cosf(angle);
		return Matrix3(cos, sin, 0,
			-sin, cos, 0,
			0, 0, 1);
	}

	Matrix3 Matrix3::getScaleMatrix(float scale)
	{
		return Matrix3(scale, 0, 0,
			0, scale, 0,
			0, 0, scale);
	}

	Matrix3 Matrix3::operator* (Matrix3 const& mat)
	{
		return Matrix3((m[0] * mat.m[0]) + (m[3] * mat.m[1]) + (m[6] * mat.m[2]),    //m11
					   (m[1] * mat.m[0]) + (m[4] * mat.m[1]) + (m[7] * mat.m[2]),    //m21
					   (m[2] * mat.m[0]) + (m[5] * mat.m[1]) + (m[8] * mat.m[2]),    //m31
					   (m[0] * mat.m[3]) + (m[3] * mat.m[4]) + (m[6] * mat.m[5]),    //m12 
					   (m[1] * mat.m[3]) + (m[4] * mat.m[4]) + (m[7] * mat.m[5]),    //m22
					   (m[2] * mat.m[3]) + (m[5] * mat.m[4]) + (m[8] * mat.m[5]),    //m32 
					   (m[0] * mat.m[6]) + (m[3] * mat.m[7]) + (m[6] * mat.m[8]),    //m13
					   (m[1] * mat.m[6]) + (m[4] * mat.m[7]) + (m[7] * mat.m[8]),    //m23
					   (m[2] * mat.m[6]) + (m[5] * mat.m[7]) + (m[8] * mat.m[8]));   //m33
	}

	Matrix3 Matrix3::operator+ (float f)
	{
		return Matrix3(m[0] + f, m[1] + f, m[2] + f,
			m[3] + f, m[4] + f, m[5] + f,
			m[6] + f, m[7] + f, m[8] + f);
	}

	Matrix3 Matrix3::operator+ (Matrix3 const& mat)
	{
		return Matrix3(m[0] + mat.m[0], m[1] + mat.m[1], m[2] + mat.m[2],
			m[3] + mat.m[3], m[4] + mat.m[4], m[5] + mat.m[5],
			m[6] + mat.m[6], m[7] + mat.m[7], m[8] + mat.m[8]);
	}

	Matrix3 Matrix3::operator- (Matrix3 const& mat)
	{
		return Matrix3(m[0] - mat.m[0], m[1] - mat.m[1], m[2] - mat.m[2],
			m[3] - mat.m[3], m[4] - mat.m[4], m[5] - mat.m[5],
			m[6] - mat.m[6], m[7] - mat.m[7], m[8] - mat.m[8]);
	}

	mlib::Vector3 operator* (const mlib::Matrix3& mat, const mlib::Vector3& vector)
	{
		return mlib::Vector3(mat.m[0] * vector.x + mat.m[3] * vector.y + mat.m[6] * vector.z,
			mat.m[1] * vector.x + mat.m[4] * vector.y + mat.m[7] * vector.z,
			mat.m[2] * vector.x + mat.m[5] * vector.y + mat.m[8] * vector.z);
	}

	mlib::Vector2 operator*(const mlib::Matrix3& mat, const mlib::Vector2& vector)
	{
		return mlib::Vector2(mat.m[0] * vector.x + mat.m[3] * vector.y,
			mat.m[1] * vector.x + mat.m[4] * vector.y);
	}
}