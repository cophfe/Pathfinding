#pragma once

#include "NecessaryHeaders.h"
#include <algorithm>
class Transform;
class GameObject;

struct TextureComplex
{
	Texture2D* texture;
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
	inline Rectangle* getSourceRectangle() { return &srcRect; }
	inline Rectangle* getDestinationRectangle() { return &destRect; }
	void UpdateSpriteRectangle();
protected:
	Transform* transform;
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
	void Draw();

	inline void setTimePerFrame(float seconds) { secondsPerFrame = seconds; }
private:
	bool paused;
	int currentFrame;
	float secondsPerFrame;
	int startFrame;
	int endFrame;
	float frameTimer = 0;
};