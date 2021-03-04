#pragma once
#include "Drawable.h"
#include <Map.h>
#include <sstream>
#include <SFML/Graphics/Shape.hpp>

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}

	struct MTV;
	class RigidBody;

	class Collider : public Drawable
	{
	private:
		RigidBody* mRigidBody = nullptr;
		bool mIsTrigger = false;

	public:
		Collider() = default;
		Collider(const Collider& other);
		Collider(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);
		virtual ~Collider() override;
		virtual void update(sf::Time delta) = 0;
		virtual void draw(sf::RenderWindow& window);
		virtual bool collidesWith(Collider* collider, MTV& mtv) = 0;
		virtual sf::Shape& getShape() = 0;

		virtual void onEnable() override;
		virtual void onDisable() override;
		virtual void onStart() override;

		RigidBody* getRigidBody();
		bool isTrigger();
		void setTrigger(bool isTrigger);
	};
}

