#pragma once

#include "NecessaryHeaders.h"
class Transform;
class GameObject;

struct TextureComplex
{
	Texture2D* textures;
	int textureNumber;
};

class Sprite
{
public:
	Sprite();
	Sprite(TextureComplex* textureComplex, GameObject* attached);

	virtual void Draw();
	void flip();
	void setFlipped(bool flippedValue);
	bool getFlipped() { return srcRect.width < 0; }
	inline void setTint(Color tint) { this->tint = tint; }
	inline Color& getTint() { return tint; }
	
	void UpdateSpriteRectangle();
protected:
	GameObject* gameObject;
	TextureComplex* texture;
	Color tint;


	Rectangle srcRect;
	Rectangle destRect;
	Vector2 pivot;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(TextureComplex* textureComplex, GameObject* attached);

	void Play();
	void Pause();
	void Draw(Transform* attached);
private:
	bool paused;
	int currentFrame;
	int msPerFrame;
	int startFrame;
	int endFrame;

};