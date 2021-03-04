#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include <map>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class GameObjectManager
	{
	public:
		static GameObjectManager& getInstance();
		void update(sf::Time delta);
		void physicsUpdate(sf::Time delta);
		void add(GameObject* go);
		void addTemplate(GameObject* go);
		void clear();
		GameObject* getObject(std::string Name);
		GameObject* cloneTemplate(std::string templateName, std::string cloneName);
		std::map<std::string, std::unique_ptr<GameObject>>& getObjects();

	private:
		GameObjectManager() = default;
		std::map<std::string, std::unique_ptr<GameObject>> mTemplates;
		std::map<std::string, std::unique_ptr<GameObject>> mObjects;
		std::vector<std::unique_ptr<GameObject>> mToAdd;
	};
}