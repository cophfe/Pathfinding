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

Sprite::Sprite(Texture2D* textureComplex, GameObject* attached)
{
	transform = attached->getTransform();
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };

	srcRect = { 0, 0, (float)texture->width, (float)texture->height};
	UpdateSpriteRectangle();
}

void Sprite::draw()
{
	DrawTexturePro(*texture, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
}

void Sprite::flip()
{
	srcRect.width *= -1;
}

void Sprite::setFlipped(bool flippedValue)
{
	srcRect.width = flippedValue ? (float)-texture->width : (float)texture->width;
}

//the destination rectangle needs to be updated every time position or scale changes
void Sprite::UpdateSpriteRectangle()
{
	Vector2& pos = transform->getGlobalPosition();
	float scale = transform->getGlobalScale();
	destRect = Rectangle{ pos.x, pos.y + drawOffset, texture->width * scale, texture->height * scale };
	pivot.x = destRect.width / 2;
	pivot.y = destRect.height / 2 + drawOffset;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		ANIMATED SPRITE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AnimatedSprite::AnimatedSprite(AnimatedTexture* textureComplex, GameObject* attached)
{
	transform = attached->getTransform();
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };
	paused = false;
	currentFrame = 0;
	startFrame = 0;
	endFrame = textureComplex->textureNumber - 1;
	secondsPerFrame = 0.03333f;

	srcRect = { 0, 0, (float)((AnimatedTexture*)texture)->spriteWidth, (float)((AnimatedTexture*)texture)->spriteHeight };
	UpdateSpriteRectangle();
}

void AnimatedSprite::play()
{
	paused = false;
}

void AnimatedSprite::pause()
{
	paused = true;
	frameTimer = 0;
}

void AnimatedSprite::pauseAt(int frame)
{
	currentFrame = frame;
	updateSrcCoordinates();
	paused = true;
	frameTimer = 0;
}

void AnimatedSprite::playAt(int frame)
{
	currentFrame = frame;
	paused = false;
}

void AnimatedSprite::setSettings(int startFrame, int endFrame, int currentFrame)
{
	this->currentFrame = currentFrame;
	this->endFrame == endFrame;
	this->startFrame = startFrame;
	frameTimer = 0;
}

void AnimatedSprite::draw()
{
	frameTimer += Game::getDeltaTime();

	while (!paused && frameTimer > secondsPerFrame)
	{
		currentFrame++;
		if (currentFrame > endFrame)
			currentFrame = startFrame;
		updateSrcCoordinates();
		frameTimer -= secondsPerFrame;
	}
	DrawTexturePro(*texture, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
}

void AnimatedSprite::UpdateSpriteRectangle()
{
	Vector2& pos = transform->getGlobalPosition();
	float scale = transform->getGlobalScale();
	destRect = Rectangle{ pos.x, pos.y, ((AnimatedTexture*)texture)->spriteWidth* scale, ((AnimatedTexture*)texture)->spriteHeight* scale };
	pivot.x = destRect.width / 2;
	pivot.y = destRect.height / 2 + drawOffset;
}

void AnimatedSprite::setFlipped(bool flippedValue)
{
	srcRect.width = flippedValue ? (float)-((AnimatedTexture*)texture)->spriteWidth : (float)((AnimatedTexture*)texture)->spriteWidth;
}
