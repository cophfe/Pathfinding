#include "Sprite.h"
#include "GameObject.h"

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
	//~~~~~~~~~~~~~~~~~~~	JUST COPY RECT STUFF FROM PREVIOUS BECAUSE TEXTUREEX SUCKS
	//DrawTextureEx(*texture->textures, transform->getGlobalPosition(), transform->getGlobalRotation() * rad2Deg, transform->getGlobalScale(), tint);
	DrawTexturePro(*texture->textures, srcRect, destRect, pivot, transform->getGlobalRotation() * rad2Deg, tint);
	DrawRectangle(transform->getGlobalPosition().x, transform->getGlobalPosition().y, 15, 15, { 0,0x22,0,0xFF });
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
	pivot.y = destRect.height/2;
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
