#pragma once
#include "Component.h"
#include <iostream>
#include "TiledMap.h"

using namespace hfm;

hfm::Component::Component()
{
}

hfm::Component::Component(const Component& other)
{
	mName = other.mName;
	mIsEnabled = other.mIsEnabled;
}

hfm::Component::Component(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
{
	mName = obj->Name;

	if (obj->Properties.find("isEnabled") != obj->Properties.end())
	{
		bool isEnabled;
		std::istringstream(obj->Properties["isEnabled"]) >> std::boolalpha >> isEnabled;
		setEnabled(isEnabled);
	}
}

bool Component::isEnabled()
{
	return mIsEnabled;
}

void Component::setEnabled(bool enabled)
{
	if (mIsEnabled != enabled)
	{
		mIsEnabled = enabled;

		if (enabled)
			onEnable();
		else
			onDisable();
	}
}

bool Component::isDestroyed()
{
	return mIsDestroyed;
}

GameObject* Component::getGameObject()
{
	return mGo;
}

std::string hfm::Component::getName()
{
	return mName;
}

Component::~Component()
{
}

Component* hfm::Component::clone()
{
	throw "Clone method for derived class not defined";
}

void Component::onAwake()
{
}

void Component::onStart()
{
}

void Component::onEnable()
{
}

void Component::onDisable()
{
}

void Component::onAdd(GameObject* go)
{
	mGo = go;
	mId = (int)this;
}

void Component::update(sf::Time)
{
}

void Component::physicsUpdate(sf::Time delta)
{
}
