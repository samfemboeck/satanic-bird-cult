#include "GameObject.h"
#include <iostream>
#include "Drawable.h"
#include "RenderManager.h"
#include "TiledMap.h"
#include <memory>
#include "GameObjectManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

int GameObject::instances = 0;

GameObject::~GameObject()
{
	for (const auto& component : mComponents)
	{
		if (component->isEnabled())
			component->onDisable();
	}

	instances--;
}

GameObject::GameObject(std::string name)
{
	mName = name;
	mId = instances++;
}

GameObject::GameObject(std::string name, tld::Object* obj)
{
	setName(name);
	setPosition(Map::Scale * obj->X, Map::Scale * obj->Y);

	if (obj->Properties.find("isEnabled") != obj->Properties.end())
	{
		bool isEnabled;
		std::istringstream(obj->Properties["isEnabled"]) >> std::boolalpha >> isEnabled;
		setEnabled(isEnabled);
	}
	
	if (obj->Properties.find("isTemplate") != obj->Properties.end())
	{
		bool isTemplate;
		std::istringstream(obj->Properties["isTemplate"]) >> std::boolalpha >> isTemplate;
		setTemplate(isTemplate);
	}

}

hfm::GameObject::GameObject(const GameObject& other)
{
	mName = other.mName;
	mId = instances++;
	mIsEnabled = other.mIsEnabled;
	setPosition(other.getPosition());
	
	for (const auto& child : other.mChildren)
	{
		GameObject* clone = new GameObject(*child);
		mChildren.push_back(std::unique_ptr<GameObject>(clone));
	}

	for (const auto& cmp : other.mComponentsToAdd)
	{
		Component* clone = cmp->clone();
		addComponent(std::unique_ptr<Component>(clone));
	}
}

bool GameObject::hasRigidbody()
{
	return mHasRigidBody;
}

std::string GameObject::getName()
{
	return mName;
}

void GameObject::setName(std::string name)
{
	mName = name;
}

int GameObject::getId()
{
	return mId;
}

void GameObject::setRigidbody(bool hasRigidBody)
{
	mHasRigidBody = hasRigidBody;
}

void GameObject::markForDelete()
{
	mIsMarkedForDelete = true;
}

bool GameObject::isMarkedForDelete()
{
	return mIsMarkedForDelete;
}

bool GameObject::isEnabled()
{
	return mIsEnabled;
}

bool hfm::GameObject::isTemplate()
{
	return mIsTemplate;
}

void hfm::GameObject::setTemplate(bool isTemplate)
{
	mIsTemplate = isTemplate;
}

void GameObject::addChild(std::unique_ptr<GameObject> go)
{
	go->setParent(this);
	mChildren.push_back(std::move(go));
}

void GameObject::setParent(GameObject* go)
{
	mParent = go;
}

void GameObject::addComponent(std::unique_ptr<Component> component)
{
	component->onAdd(this);
	mComponentsToAdd.push_back(std::move(component));
}

void GameObject::update(sf::Time delta)
{
	if (mParent)
		setPosition(mParent->getPosition());

	std::vector<Component*> componentsToAdd;

	for (auto& component : mComponentsToAdd)
	{
		componentsToAdd.push_back(component.get());
		mComponents.push_back(std::move(component));
	}

	for (const auto& component : componentsToAdd)
	{
		if (mIsEnabled && component->isEnabled())
			component->onEnable();
	}

	for (const auto& component : componentsToAdd)
	{
		component->onAwake();
	}

	for (const auto& component : componentsToAdd)
	{
		component->onStart();
	}

	mComponentsToAdd.clear();

	for (const auto& component : mComponents)
	{
		if (component->isEnabled())
			component->update(delta);
	}

	for (const auto& child : mChildren)
	{
		child->update(delta);
	}
}

void GameObject::physicsUpdate(sf::Time delta)
{
	for (const auto& cmp : mComponents)
	{
		if (cmp->isEnabled())
			cmp->physicsUpdate(delta);
	}
}

void GameObject::setEnabled(bool enabled)
{
	if (enabled != mIsEnabled)
	{
		mIsEnabled = enabled;

		for (const auto& component : mComponents)
		{
			component->setEnabled(enabled);
		}
	}
}
