#pragma once
#include "Collider.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "TiledMap.h"

namespace hfm
{
	class CircleCollider : public hfm::Collider
	{
	public:
		static bool vsCircle(sf::CircleShape& a, sf::CircleShape& b, MTV& mtv);

		CircleCollider() = delete;
		CircleCollider(const CircleCollider& other);
		CircleCollider(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

		CircleCollider* clone() override;
		bool collidesWith(Collider* collider, MTV& mtv) override;
		void update(sf::Time) override;
		sf::Shape& getShape() override;

	private:
		sf::CircleShape mShape;
	};
}


