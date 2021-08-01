#pragma once

#include "NecessaryHeaders.h"
#include <algorithm>
class Transform;
class GameObject;

//texture for animated sprites, inherits from texture2D
//holds info for rendering animated sprites
struct AnimatedTexture : public Texture2D
{
	//data inside of Texture2D
	/*unsigned int id;
	int width;
	int height;
	int mipmaps;
	int format;*/
	int textureNumber;
	int spriteWidth, spriteHeight;
	Vector2i* coordinates;
};

//There is a sprite attached to every GameObject
//used for rendering
class Sprite
{
public:
	Sprite();
	Sprite(Texture2D* textureComplex, GameObject* attached);

	virtual void draw();
	//rectangle needs to be updated when player transform changes in any way
	virtual void updateSpriteRectangle();
	//check if is nullsprite
	virtual bool isNull() { return false; }

	//getters and setters
	void flip();
	virtual void setFlipped(bool flippedValue);
	bool getFlipped() { return srcRect.width < 0; }
	inline void setTint(Color tint) { this->tint = tint; }
	inline Color& getTint() { return tint; }
	inline Rectangle* getSourceRectangle() { return &srcRect; }
	inline Rectangle* getDestinationRectangle() { return &destRect; }
	float getWidth() { return destRect.width; }
	float getHeight() { return destRect.height; }
	void setDrawOffset(float offset) { this->offset = offset; }
	float getDrawOffset() { return offset; }
	void setShader(const char* name);
	void setShader(Shader* shader) { this->shader = shader; }
	void clearShader() { this->shader = nullptr; }
	Shader* getShader() { return shader; }
	Texture* getTexture() { return texture; }

protected:
	//internal rendering stuff
	Rectangle srcRect;
	Rectangle destRect;
	Vector2 pivot;
	Transform* transform;
	Texture2D* texture;
	//can add fragment shader
	Shader* shader = nullptr;
	Color tint;
	//used to move up or down on y axis from the transform position
	float offset = 0;
};

//used to draw sprite sheets
class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(AnimatedTexture* textureComplex, GameObject* attached);

	void draw();
	void updateSpriteRectangle();

	//these are all very self explanatory
	void play();
	void pause();
	void pauseAt(int frame);
	void playAt(int frame);
	void setSettings(int startFrame, int endFrame, int currentFrame);
	void setCallback(void(*function)(void*), void* callbackPointer);
	void setFlipped(bool flippedValue);
	
	//getters and setters
	inline void setTimePerFrame(float seconds) { secondsPerFrame = seconds; }
	inline void setCurrentFrame(int currentFrame) { this->currentFrame = currentFrame; updateSrcCoordinates(); }
	inline int getEndFrame() { return startFrame; }
	inline int getStartFrame() { return endFrame; }
	inline int getCurrentFrame() { return currentFrame; }

private:
	//used to update where in the sprite sheet the render is sampling from
	inline void updateSrcCoordinates() {
		srcRect.x = (float)((AnimatedTexture*)texture)->coordinates[currentFrame].x;
		srcRect.y = (float)((AnimatedTexture*)texture)->coordinates[currentFrame].y;
	}

	//callback is run at the end of animation loop
	void* callbackPointer;
	void(*endCallback)(void*);
	
	//data for animation
	float secondsPerFrame;
	float frameTimer = 0;
	int startFrame;
	int currentFrame;
	int endFrame;
	bool paused;
};

// Easy way to get out of checking if a sprite is null
// If a gameObject isn't supposed to be drawn, it's sprite reference will be to a nullsprite
// if a gameobject is supposed to be drawn but the intended texture is not found, 
// it's sprite will be a 'missing' texture
class NullSprite : public Sprite
{
public:
	NullSprite() {
		texture = { 0 };
		tint = { 0 };
		srcRect = { 0 };
		destRect = { 0 };
	}
	void draw() {}
	void updateSpriteRectangle() {}
	bool isNull() { return true; }
};

typedef void(*RenderCallback)(void*);

//this just calls a render callback in the draw function
class CustomRenderSprite : public Sprite
{
public:
	CustomRenderSprite(RenderCallback callback, void* pointer) : callback(callback), pointer(pointer) {}
	void draw();

	inline void setCallback(RenderCallback callback) { this->callback = callback; }
	inline void setPointer(void* pointer) { this->pointer = pointer; }
	void updateSpriteRectangle() {}
private:
	RenderCallback callback;
	void* pointer;
};