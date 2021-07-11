#include "Sprite.h"
#include "GameObject.h"

Sprite::Sprite()
{
}

Sprite::Sprite(TextureComplex* textureComplex, GameObject* attached)
{
	gameObject = attached;
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };

	srcRect = { 0, 0, (float)texture->textures->width, (float)texture->textures->height};
	UpdateSpriteRectangle();
}

void Sprite::Draw()
{
	//~~~~~~~~~~~~~~~~~~~	JUST COPY RECT STUFF FROM PREVIOUS BECAUSE TEXTUREEX SUCKS
	//DrawTextureEx(*texture->textures, gameObject->transform.getGlobalPosition(), gameObject->transform.getGlobalRotation() * rad2Deg, gameObject->transform.getGlobalScale(), tint);
	DrawTexturePro(*texture->textures, srcRect, destRect, pivot, gameObject->transform->getGlobalRotation() * rad2Deg, tint);
}

void Sprite::flip()
{
	srcRect.width *= -1;
}

void Sprite::setFlipped(bool flippedValue)
{
	srcRect.width = flippedValue ? -texture->textures->width : texture->textures->width;
}

//the destination rectangle needs to be updated every time position or scale changes
void Sprite::UpdateSpriteRectangle()
{
	Vector2& pos = gameObject->transform->getGlobalPosition();
	float scale = gameObject->transform->getGlobalScale();
	destRect = Rectangle{ pos.x, pos.y, texture->textures->width * scale, texture->textures->height * scale };
	pivot.x = destRect.width / 2;
	pivot.y = destRect.height / 2;
}

AnimatedSprite::AnimatedSprite(TextureComplex* textureComplex, GameObject* attached)
{
	gameObject = attached;
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };
	paused = false;
	currentFrame = 0;
	startFrame = 0;
	endFrame = texture->textureNumber - 1;
	msPerFrame = 30;
}

void AnimatedSprite::Play()
{
}

void AnimatedSprite::Pause()
{
}

void AnimatedSprite::Draw(Transform* attached)
{
}
