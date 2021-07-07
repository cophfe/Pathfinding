#pragma once

#include "NecessaryHeaders.h"
class GameObject;

class Sprite
{
public:
	enum Layer
	{
		BACKGROUND,
		MIDGROUND,
		FOREGROUND
	};

	virtual void Draw(GameObject* attached);
protected:
	Texture2D* texture;
};

