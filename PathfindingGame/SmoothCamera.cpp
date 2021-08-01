#include "SmoothCamera.h"
#include "Game.h"
#include "Transform.h"

#define CAMERA_SCALE 0.5f

SmoothCamera::SmoothCamera(Vector2 initialPosition, float initialRotation, float initialZoom, Vector2 offset, float smoothMultiplier, Rectangle bounds, bool enabled)
{
	setBounds(bounds);

	this->offset = Vector2(GetScreenWidth() / 2 + offset.x, GetScreenHeight() / 2 + offset.y);
	camera = Camera2D{ initialPosition + Vector2(GetScreenWidth() / 2 + offset.x, GetScreenHeight() / 2 + offset.y), initialPosition, initialRotation, initialZoom * CAMERA_SCALE };
	on = true;
	targetTransform = nullptr;
	this->smoothMultiplier = smoothMultiplier;
	rotation = initialRotation;
	camera.offset.x = this->offset.x;
	camera.offset.y = this->offset.y;
	this->on = enabled;
}

void SmoothCamera::updateCamera()
{
	if (on == false || targetTransform == nullptr)
		return;

	auto& pos = targetTransform->getGlobalPosition();
	if (!(camera.target.x == pos.x && camera.target.y == pos.y))
	{
		//move camera target toward targetTransform
		//camera is updated on fixed time step so fixed delta time (using it just in case I change it at some point)
		camera.target.x += (pos.x - camera.target.x) * PHYSICS_TIME_STEP * smoothMultiplier;
		camera.target.y += (pos.y - camera.target.y) * PHYSICS_TIME_STEP * smoothMultiplier;
		
		//clamp to inside bounds
		restrictToBounds();
	}
}

void SmoothCamera::startCamera()
{
	BeginMode2D(camera);
}

void SmoothCamera::endCamera()
{
	EndMode2D();
}

void SmoothCamera::setBounds(Rectangle rect)
{
	boundsTopLeft = Vector2(rect.x, rect.y);
	boundsBottomRight = Vector2(rect.x + rect.width, rect.y + rect.height);;
	hasBoundsX = rect.width >= 0;
	hasBoundsY = rect.height >= 0;
}

void SmoothCamera::restrictToBounds()
{
	//simple clamping
	if (hasBoundsX)
	{
		if (camera.target.x > boundsBottomRight.x)
			camera.target.x = boundsBottomRight.x;
		if (camera.target.x < boundsTopLeft.x)
			camera.target.x = boundsTopLeft.x;
	}
	if (hasBoundsY)
	{

		if (camera.target.y > boundsBottomRight.y)
			camera.target.y = boundsBottomRight.y;
		if (camera.target.y < boundsTopLeft.y)
			camera.target.y = boundsTopLeft.y;
	}
}

void SmoothCamera::target(Transform* transform)
{
	targetTransform = transform;
}

void SmoothCamera::setPosition(Vector2 position)
{
	camera.target.x = (position.x - camera.target.x);
	camera.target.y = (position.y - camera.target.y);
}

Vector2 SmoothCamera::getCameraMousePosition()
{
	RLVector2 mousePos = GetMousePosition();
	return { (mousePos.x - camera.offset.x) / camera.zoom + camera.target.x, (mousePos.y - camera.offset.y) / camera.zoom + camera.target.y };
}

void SmoothCamera::setOffset(Vector2 offset)
{
	this->offset = Vector2(GetScreenWidth() / 2 + offset.x, GetScreenHeight() / 2 + offset.y);
	camera.offset.x = this->offset.x;
	camera.offset.y = this->offset.y;
}
