#include "Transform.h"

Transform::Transform()
{
}

Transform::Transform(Vector2 position, float scale, float rotation, Transform* parent, GameObject* attachedGameObject)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->parent = parent;
	this->gameObject = attachedGameObject;

	updateTransforms();
}

void Transform::addRotation(float rad)
{
	updateTransforms();
	rotation += rad;
	rotation = fmodf(rotation, 2 * pi);
}

void Transform::addPosition(Vector2* pos)
{
	updateTransforms();
}

void Transform::addScale(float scale)
{
	updateTransforms();
}

void Transform::setRotation(float rad)
{
	updateTransforms();
}

void Transform::setPosition(Vector2* pos)
{
	updateTransforms();
}

void Transform::setScale(float scale)
{
	updateTransforms();
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

void Transform::updateTransforms()
{
	localTransform = Matrix3::getPositionMatrix(position) * Matrix3::getRotationMatrix2D(rotation) * Matrix3::getScaleMatrix(scale);
	if (parent = nullptr)
	{
		globalTransform = localTransform;
	}
	else
	{
		globalTransform = localTransform * parent->getGlobalTransform();
	}

	for (auto& child : children)
	{
		child->updateTransforms();
	}
}

void Transform::addChild(Transform* child)
{
}

void Transform::setParent(Transform* parent)
{
}

void Transform::removeChild(Transform* child)
{
}

GameObject* Transform::getGameObject()
{
	return nullptr;
}
