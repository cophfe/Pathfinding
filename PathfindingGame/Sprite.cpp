#include "Sprite.h"
#include "GameObject.h"
#include "Game.h"

Sprite::Sprite()
{
	destRect = { 0 };
	srcRect = { 0 };
	texture = nullptr;
	transform = nullptr;
	tint = { 0 };

}

Sprite::Sprite(TextureComplex* textureComplex, GameObject* attached)
{
	transform = attached->getTransform();
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };

	srcRect = { 0, 0, (float)texture->textures->width, (float)texture->textures->height};
	UpdateSpriteRectangle();
}

void Sprite::Draw()
{
	DrawTexturePro(*texture->textures, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
}

void Sprite::flip()
{
	srcRect.width *= -1;
}

void Sprite::setFlipped(bool flippedValue)
{
	srcRect.width = flippedValue ? (float)-texture->textures->width : (float)texture->textures->width;
}

//the destination rectangle needs to be updated every time position or scale changes
void Sprite::UpdateSpriteRectangle()
{
	Vector2& pos = transform->getGlobalPosition();
	float scale = transform->getGlobalScale();
	destRect = Rectangle{ pos.x, pos.y, texture->textures->width * scale, texture->textures->height * scale };
	pivot.x = destRect.width / 2;
	pivot.y = destRect.height / 2;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		ANIMATED SPRITE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AnimatedSprite::AnimatedSprite(TextureComplex* textureComplex, GameObject* attached)
{
	transform = attached->getTransform();
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };
	paused = false;
	currentFrame = 0;
	startFrame = 0;
	endFrame = texture->textureNumber - 1;
	secondsPerFrame = 0.03333f;

	srcRect = { 0, 0, (float)texture->textures->width, (float)texture->textures->height };
	UpdateSpriteRectangle();
}

void AnimatedSprite::Play()
{
	paused = false;
	frameTimer = 0;
}

void AnimatedSprite::Pause()
{
	paused = true;
}

void AnimatedSprite::Draw()
{
	frameTimer += Game::getDeltaTime();

	while (!paused && frameTimer > secondsPerFrame)
	{
		currentFrame++;
		if (currentFrame > endFrame)
			currentFrame = startFrame;
		frameTimer -= secondsPerFrame;
	}
	DrawTexturePro(texture->textures[currentFrame], srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
}
