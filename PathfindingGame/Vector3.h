#pragma once
namespace mlib
{
	//simple vector3 class
	struct Vector3
	{
		float x, y, z;

		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
	};
}

