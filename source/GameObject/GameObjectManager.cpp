#include "GameObjectManager.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

GameObjectManager& GameObjectManager::getInstance()
{
	static GameObjectManager instance;
	return instance;
}

void GameObjectManager::update(sf::Time delta)
{
	auto& templates = mTemplates;
	auto it = mObjects.begin();
	while (it != mObjects.end())
	{
		auto& go = it->second;
		if (go->isMarkedForDelete())
		{
			it = mObjects.erase(it);
		}
		else
		{
			if (go->isEnabled())
				go->update(delta);

			++it;
		}
	}
}

void GameObjectManager::physicsUpdate(sf::Time delta)
{
	for (const auto& pair : mObjects)
	{
		pair.second->physicsUpdate(delta);
	}
}

void GameObjectManager::add(GameObject* go)
{
	if (mObjects.find(go->getName()) != mObjects.end())
		throw "Duplicate GameObject names are not allowed.";

	mObjects[go->getName()] = std::unique_ptr<GameObject>(go);
}

void GameObjectManager::addTemplate(GameObject* go)
{
	mTemplates[go->getName()] = std::unique_ptr<GameObject>(go);
}

void GameObjectManager::clear()
{
	mObjects.clear();
}

GameObject* GameObjectManager::getObject(std::string name)
{
	return mObjects[name].get();
}

GameObject* hfm::GameObjectManager::cloneTemplate(std::string templateName, std::string cloneName)
{
	GameObject* tmplt = mTemplates[templateName].get();
	auto clone =  new GameObject(*tmplt);
	clone->setName(cloneName);
	clone->update(sf::seconds(0));
	add(clone);
	return clone;
}

std::map<std::string, std::unique_ptr<GameObject>>& GameObjectManager::getObjects()
{
	return mObjects;
}
