#pragma once
#include "NecessaryHeaders.h"
class GameObject;
#include "Transform.h"

class Component
{
public:
	virtual ~Component() = default;
	virtual void init(GameObject* connected);
	virtual void start();
	virtual void update();
	virtual void onDisable();
	virtual void onEnable();
	virtual void unload();


	void disableComponent();
	void enableComponent();

	inline bool isEnabled() { return enabled; }

protected:
	GameObject* gameObject;
	Transform* transform;
	bool enabled;
};

