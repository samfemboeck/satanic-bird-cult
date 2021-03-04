#include "Collider.h"
#include "PhysicsManager.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "TiledMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

Collider::Collider(const Collider& other) : Drawable(other)
{
	mIsTrigger = other.mIsTrigger;
}

Collider::Collider(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Drawable(obj, group, go)
{
	std::map<string, string>::iterator it;
	if ((it = obj->Properties.find("isTrigger")) != obj->Properties.end())
	{
		bool isTrigger;
		std::string val = it->second;
		std::istringstream(val) >> std::boolalpha >> isTrigger;
		setTrigger(isTrigger);
	}
}

Collider::~Collider()
{
}

void Collider::draw(sf::RenderWindow& window)
{
	window.draw(getShape());
}

void Collider::onEnable()
{
	Drawable::onEnable();
	PhysicsManager::getInstance().add(this);
}

void Collider::onDisable()
{
	Drawable::onDisable();
	PhysicsManager::getInstance().remove(this);
}

void Collider::onStart()
{
	mRigidBody = mGo->getComponent<RigidBody>();
}

RigidBody* Collider::getRigidBody()
{
	return mRigidBody;
}

bool Collider::isTrigger()
{
	return mIsTrigger;
}

void Collider::setTrigger(bool isTrigger)
{
	mIsTrigger = isTrigger;
}