#include "Sprite.h"
#include "GameObject.h"

void Sprite::Draw(GameObject* attached)
{
	//will get all transformation out of the global transform
	DrawTextureEx(*texture, attached->position, attached->rotation, attached->scale, attached->tint);
}
