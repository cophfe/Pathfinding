#include "SmoothCamera.h"
#include "Game.h"
#include "Transform.h"

#define CAMERA_SCALE 0.5f

SmoothCamera::SmoothCamera(Vector2 initialPosition, float initialRotation, float initialZoom, Vector2 offset, float smoothMultiplier)
{
	this->offset = Vector2(GetScreenWidth() / 2 + offset.x, GetScreenHeight() / 2 + offset.y);
	camera = Camera2D{ initialPosition + Vector2(GetScreenWidth() / 2 + offset.x, GetScreenHeight() / 2 + offset.y), initialPosition, initialRotation, initialZoom * CAMERA_SCALE };
	on = true;
	target = nullptr;
	this->smoothMultiplier = smoothMultiplier;
	rotation = initialRotation;
	camera.offset.x = this->offset.x;
	camera.offset.y = this->offset.y;
}

void SmoothCamera::UpdateCamera()
{
	if (on == false || target == nullptr)
		return;

	auto& pos = target->getGlobalPosition();
	if (!(camera.target.x == pos.x && camera.target.y == pos.y))
	{
		camera.target.x += (pos.x - camera.target.x) * PHYSICS_TIME_STEP * smoothMultiplier;
		camera.target.y += (pos.y - camera.target.y) * PHYSICS_TIME_STEP * smoothMultiplier;
		//camera.target = pos;
	}

	//camera.rotation = target->getGlobalRotation() * -rad2Deg;
}

void SmoothCamera::StartCamera()
{
	BeginMode2D(camera);
}

void SmoothCamera::EndCamera()
{
	EndMode2D();
}

void SmoothCamera::Target(Transform* transform)
{
	target = transform;
}

Vector2 SmoothCamera::GetCameraMousePosition()
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
