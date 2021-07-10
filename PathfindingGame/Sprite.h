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
	enum Layer
	{
		BACKGROUND,
		MIDGROUND,
		FOREGROUND
	};

	Sprite();
	Sprite(TextureComplex* textureComplex, GameObject* attached);

	virtual void Draw();
	inline void flip() { flipped = !flipped; }
	inline void setFlipped(bool flippedValue) { flipped = flippedValue; }
	inline void setTint(Color tint) { this->tint = tint; }
	inline Color& getTint() { return tint; }
protected:
	GameObject* gameObject;
	TextureComplex* texture;
	Color tint;
	bool flipped;
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