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
	updateSpriteRectangle();
}

void Sprite::draw()
{
	if (shader != nullptr)
	{
		BeginShaderMode(*shader);
		DrawTexturePro(*texture, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
		EndShaderMode();
	}
	else
	{
		DrawTexturePro(*texture, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
	}
}

void Sprite::flip()
{
	srcRect.width *= -1;
}

void Sprite::setFlipped(bool flippedValue)
{
	srcRect.width = flippedValue ? (float)-texture->width : (float)texture->width;
}

void Sprite::setShader(const char* name)
{
	shader = Game::getInstance().getTextureManager()->getShader(name);
}

//the destination rectangle needs to be updated every time position or scale changes
void Sprite::updateSpriteRectangle()
{
	Vector2& pos = transform->getGlobalPosition();
	float scale = transform->getGlobalScale();
	destRect = Rectangle{ pos.x, pos.y + offset, texture->width * scale, texture->height * scale };
	pivot.x = destRect.width / 2;
	pivot.y = destRect.height / 2 + offset;
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
	updateSpriteRectangle();

	endCallback = nullptr;
	callbackPointer = nullptr;
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
	this->endFrame = endFrame;
	this->startFrame = startFrame;
	frameTimer = 0;
}

void AnimatedSprite::setCallback(void(*function)(void*), void* callbackPointer)
{
	endCallback = function;
	this->callbackPointer = callbackPointer;
}

void AnimatedSprite::draw()
{
	frameTimer += Game::getDeltaTime();

	//this changes frame whenever necessary
	while (!paused && frameTimer > secondsPerFrame)
	{
		currentFrame++;
		if (currentFrame > endFrame)
		{
			currentFrame = startFrame;
			//call endCallback before restarting loop
			if (endCallback != nullptr)
			{
				endCallback(callbackPointer);
			}
		}
		updateSrcCoordinates();
		frameTimer -= secondsPerFrame;
	}
	
	if (shader != nullptr)
	{
		BeginShaderMode(*shader);
		DrawTexturePro(*texture, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
		EndShaderMode();
	}
	else
	{
		DrawTexturePro(*texture, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
	}
}

void AnimatedSprite::updateSpriteRectangle()
{
	Vector2& pos = transform->getGlobalPosition();
	float scale = transform->getGlobalScale();
	destRect = Rectangle{ pos.x, pos.y, ((AnimatedTexture*)texture)->spriteWidth* scale, ((AnimatedTexture*)texture)->spriteHeight* scale };
	pivot.x = destRect.width / 2;
	pivot.y = destRect.height / 2 + offset;
}

void AnimatedSprite::setFlipped(bool flippedValue)
{
	srcRect.width = flippedValue ? (float)-((AnimatedTexture*)texture)->spriteWidth : (float)((AnimatedTexture*)texture)->spriteWidth;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		CUSTOM RENDER SPRITE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CustomRenderSprite::draw()
{
	callback(pointer);
}
