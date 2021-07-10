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
	flipped = false;
}

void Sprite::Draw()
{
	//will get all transformation out of the global transform
	DrawTextureEx(*texture->textures, gameObject->transform.getGlobalPosition(), gameObject->transform.getGlobalRotation(), gameObject->transform.getGlobalScale(), tint);
}

AnimatedSprite::AnimatedSprite(TextureComplex* textureComplex, GameObject* attached)
{
	gameObject = attached;
	texture = textureComplex;
	tint = { 0xFF,0xFF,0xFF,0xFF };
	flipped = false;
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

void AnimatedSprite::Draw(TransformObject* attached)
{
}
