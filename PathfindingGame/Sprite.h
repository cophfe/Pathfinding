#pragma once

#include "NecessaryHeaders.h"
#include <algorithm>
class Transform;
class GameObject;

struct AnimatedTexture : public Texture2D
{

	/*unsigned int id;
	int width;
	int height;
	int mipmaps;
	int format;*/
	int textureNumber;
	int spriteWidth, spriteHeight;
	Vector2i* coordinates;

	bool isAnimated() { return true; }
};

class Sprite
{
public:
	Sprite();
	Sprite(Texture2D* textureComplex, GameObject* attached);

	virtual void Draw();
	virtual void UpdateSpriteRectangle();
	virtual bool isNull() { return false; }
	void flip();
	void setFlipped(bool flippedValue);
	bool getFlipped() { return srcRect.width < 0; }
	inline void setTint(Color tint) { this->tint = tint; }
	inline Color& getTint() { return tint; }
	inline Rectangle* getSourceRectangle() { return &srcRect; }
	inline Rectangle* getDestinationRectangle() { return &destRect; }
	float getWidth() { return destRect.width; }
	float getHeight() { return destRect.height; }
	void setDrawOffset(float drawOffset) { this->drawOffset = drawOffset; }
	float drawOffset = 0;
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

// Easy way to get out of checking if a sprite is null
// If a gameObject isn't supposed to be drawn, it's sprite reference will be to a nullsprite
// if a gameobject is supposed to be drawn but the intended texture is not found, 
// it's sprite will be a 'missing' texture (which should be generated by the game, but probably will just be another image because time and effort)
class NullSprite : public Sprite
{
public:
	NullSprite() {
		texture = { 0 };
		tint = { 0 };
		srcRect = { 0 };
		destRect = { 0 };
	}
	void Draw() {}
	void UpdateSpriteRectangle() {}
	bool isNull() { return true; }
};