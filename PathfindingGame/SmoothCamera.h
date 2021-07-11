#pragma once
#include "NecessaryHeaders.h"
class Transform;

class SmoothCamera
{
public:
	SmoothCamera(Vector2 position, float rotation, float zoom, Vector2 offset, float smoothMultiplier);
	void UpdateCamera();
	void StartCamera();
	void EndCamera();
	void Target(Transform* transform);

	void setOffset(Vector2 offset);
	Vector2 getOffset() { return offset; }
	float smoothMultiplier;
private:
	Camera2D camera;
	Vector2 offset;
	Transform* target;
	float rotation;
	bool on;
};

