#pragma once
#include "Collider.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include "TiledMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class CircleCollider;

	class PolygonCollider : public Collider
	{
	public:
		static bool vsPoly(const std::vector<sf::Vector2f>& verticesA, const std::vector<sf::Vector2f>& axesA, const sf::Vector2f& centerA,
			const std::vector<sf::Vector2f>& verticesB, const std::vector<sf::Vector2f>& axesB, const sf::Vector2f& centerB, MTV& mtv);
		static bool vsCircle(PolygonCollider& poly, CircleCollider& circle, MTV& mtv);

		PolygonCollider(const PolygonCollider& other);
		PolygonCollider(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);
		PolygonCollider() = delete;

		PolygonCollider* clone() override;
		void update(sf::Time delta) override;
		void onStart() override;
		bool collidesWith(Collider* collider, MTV& mtv) override;
		sf::Shape& getShape() override;
		sf::Vector2f getCenter();
		std::vector<sf::Vector2f> getVertices();
		std::vector<sf::Vector2f> getAxes();

	private:
		sf::ConvexShape mShape;
	};
}

