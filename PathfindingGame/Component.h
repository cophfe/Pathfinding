#pragma once
#include "NecessaryHeaders.h"
class GameObject;
#include "Transform.h"

class Component
{
public:
	Component();
	virtual ~Component() = default;
	virtual void init();
	virtual void start();
	virtual void update();
	virtual void fixedUpdate();
	virtual void onDisable();
	virtual void onEnable();
	virtual void unload();

#ifdef DRAW_DEBUG
	virtual void debugDraw() {};
#endif // DRAW_DEBUG



	void setGameObject(GameObject* connected);

	void disableComponent();
	void enableComponent();

	inline bool isEnabled() { return enabled; }
	inline const Transform* getTransform() { return transform; }
	inline const GameObject* getGameObject() { return gameObject; }

protected:
	GameObject* gameObject;
	Transform* transform;
	bool enabled;
};

