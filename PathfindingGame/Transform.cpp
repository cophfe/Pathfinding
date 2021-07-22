#include "Transform.h"
#include "GameObject.h"
Transform::Transform(Vector2 position, float scale, float rotation, Transform* parent, GameObject* attachedGameObject)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->gameObject = attachedGameObject;
	this->parent = nullptr;

	localTransform = { 7, 3, 2, 1, 0, 9, 12, 15,69 };
	if (parent != nullptr)
		parent->addChild(this);

	updateLocalTransform();
}

void Transform::addRotation(float rad)
{
	rotation += rad;
	rotation = fmodf(rotation, 2 * pi);
	updateLocalTransform();
}

void Transform::addPosition(Vector2 pos)
{
	position = position + pos;
	updateLocalTransform();
	gameObject->getSprite()->UpdateSpriteRectangle();
}

void Transform::addScale(float scale)
{
	this->scale += scale;
	updateLocalTransform();
	gameObject->getSprite()->UpdateSpriteRectangle();
}

void Transform::setRotation(float rad)
{
	rotation = rad;
	updateLocalTransform();
}

void Transform::setPosition(Vector2 pos)
{
	position = pos;
	updateLocalTransform();
	gameObject->getSprite()->UpdateSpriteRectangle();
}

void Transform::setScale(float scale)
{
	this->scale = scale;
	updateLocalTransform();
	gameObject->getSprite()->UpdateSpriteRectangle();
}

Vector2& Transform::getGlobalPosition()
{
	return globalPosition;
}

float Transform::getGlobalRotation()
{
	return globalRotation;
}

float Transform::getGlobalScale()
{
	return globalScale;
}

Matrix3& Transform::getGlobalTransform()
{
	return globalTransform;
}

Matrix3& Transform::getLocalTransform()
{
	return localTransform;
}

void Transform::updateLocalTransform()
{
	localTransform = Matrix3::getPositionMatrix(position) * Matrix3::getRotationMatrix2D(rotation) * Matrix3::getScaleMatrix(scale);
	updateGlobalTransform();
}

void Transform::updateGlobalTransform()
{
	//transforms are only updated when necessary, like if position changes

	if (parent == nullptr)
	{
		globalTransform = localTransform;
		globalPosition = position;
		globalRotation = rotation;
		globalScale = scale;
	}
	else
	{
		globalTransform = parent->getGlobalTransform() * localTransform;
		globalTransform.getAllTransformations(&globalPosition, &globalScale, &globalRotation);
	}

	for (auto& child : children)
	{
		child->updateGlobalTransform();
		child->gameObject->getSprite()->UpdateSpriteRectangle();
	}
}

void Transform::flipPositionX()
{
	position.x *= -1;
	updateLocalTransform();
}

void Transform::addChild(Transform* child)
{
	children.push_front(child);
	child->setParent(this);
}

void Transform::setParent(Transform* parent)
{
	if (this->parent != nullptr)
		this->parent->removeChild(this);
	this->parent = parent;
}

void Transform::removeChild(Transform* child)
{
	child->parent = nullptr;
	children.remove(child);
}

GameObject* Transform::getGameObject()
{
	return gameObject;
}
