#pragma once
#include "NecessaryHeaders.h"
class Transform;

class SmoothCamera
{
public:
	SmoothCamera(Vector2 position, float rotation, float zoom, Vector2 offset, float smoothMultiplier, Rectangle bounds = { 0 }, bool enabled = true);
	void UpdateCamera();
	void StartCamera();
	void EndCamera();
	void setBounds(Rectangle rect);
	void Target(Transform* transform);
	void setPosition(Vector2 position);
	inline int getScale() { return camera.zoom; }
	Vector2 GetCameraMousePosition();

	void setOffset(Vector2 offset);
	Vector2 getOffset() { return offset; }
	inline void setEnabled(bool value) { on = value; }
	float smoothMultiplier;
private:
	bool hasBoundsX = false;
	bool hasBoundsY = false;
	Vector2 boundsTopLeft;
	Vector2 boundsBottomRight;

	Camera2D camera;
	Vector2 offset;
	Transform* target;

	float rotation;
	bool on;
};

