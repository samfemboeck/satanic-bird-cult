#pragma once
#include <string>
#include <SFML\Graphics\RenderWindow.hpp>

namespace hfm
{
	class GameObject;

	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}

	class Component
	{
	public:
		Component();
		Component(const Component& other);
		Component(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

		virtual ~Component();
		virtual Component* clone();
		virtual void onAdd(GameObject* go);
		virtual void onAwake();
		virtual void onStart();
		virtual void onEnable();
		virtual void onDisable();
		virtual void update(sf::Time delta);
		virtual void physicsUpdate(sf::Time delta);

		bool isEnabled();
		void setEnabled(bool enabled);
		bool isDestroyed();
		GameObject* getGameObject();
		std::string getName();

	protected:
		GameObject* mGo = nullptr;

	private:
		int mId = 0;
		bool mIsEnabled = true;
		bool mIsDestroyed = false;
		std::string mName;
	};
}
