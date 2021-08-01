#pragma once
#include "NecessaryHeaders.h"
class Transform;

//Wrapper for camera that makes movement more controlled
class SmoothCamera
{
public:
	//Constructor
	SmoothCamera(Vector2 position, float rotation, float zoom, Vector2 offset, float smoothMultiplier, Rectangle bounds = { 0 }, bool enabled = true);
	//camera position is updated every frame here
	void updateCamera();
	//initiate camera
	void startCamera();
	//end camera
	void endCamera();
	//set the bounds the camera has to stay in
	void setBounds(Rectangle rect);
	//restrict to bounds (if there are any)
	void restrictToBounds();
	//target a transform
	void target(Transform* transform);
	//set the camera position
	void setPosition(Vector2 position);
	//get the zoom level of the camera
	inline float getScale() { return camera.zoom; }
	//get the mouse position from the camera's perspective
	Vector2 getCameraMousePosition();
	//setters and getters
	void setOffset(Vector2 offset);
	Vector2 getOffset() { return offset; }
	inline void setEnabled(bool value) { on = value; }
	inline Camera2D* getCamera() { return &camera; }
	//smooth multipler can be changed because whatever who cares
	//it effects how smooth the camera movements are
	float smoothMultiplier;
private:
	//camera data
	Camera2D camera;
	Vector2 offset;
	Transform* targetTransform;

	//bounds data
	Vector2 boundsTopLeft;
	Vector2 boundsBottomRight;
	bool hasBoundsX = false;
	bool hasBoundsY = false;

	float rotation;
	//enabled
	bool on;
};

