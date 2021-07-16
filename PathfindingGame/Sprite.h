#pragma once

#include "NecessaryHeaders.h"
#include <algorithm>
class Transform;
class GameObject;

struct AnimatedTexture : public Texture2D
{
	struct COORD
	{
		int x, y;
	};

	/*unsigned int id;
	int width;
	int height;
	int mipmaps;
	int format;*/
	int textureNumber;
	int spriteWidth, spriteHeight;
	COORD* coordinates;

	bool isAnimated() { return true; }
};

class Sprite
{
public:
	Sprite();
	Sprite(Texture2D* textureComplex, GameObject* attached);

	virtual void Draw();
	void flip();
	void setFlipped(bool flippedValue);
	bool getFlipped() { return srcRect.width < 0; }
	inline void setTint(Color tint) { this->tint = tint; }
	inline Color& getTint() { return tint; }
	inline Rectangle* getSourceRectangle() { return &srcRect; }
	inline Rectangle* getDestinationRectangle() { return &destRect; }
	virtual void UpdateSpriteRectangle();
protected:
	Transform* transform;
	Texture2D* texture;
	Color tint;

	Rectangle srcRect;
	Rectangle destRect;
	Vector2 pivot;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(AnimatedTexture* textureComplex, GameObject* attached);

	void Play();
	void Pause();
	void Draw();

	inline void setTimePerFrame(float seconds) { secondsPerFrame = seconds; }
	void UpdateSpriteRectangle();
private:
	bool paused;
	int currentFrame;
	float secondsPerFrame;
	int startFrame;
	int endFrame;
	float frameTimer = 0;
};