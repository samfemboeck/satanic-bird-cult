#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class Collider;
	class RigidBody;

	struct MTV
	{
		sf::Vector2f normal;
		float penetration;
	};

	struct Manifold
	{
		RigidBody* rb1;
		RigidBody* rb2;
		MTV mtv;
	};

	class PhysicsManager
	{
	public:
		static PhysicsManager& getInstance();
		void add(Collider* collider);
		void add(RigidBody* rb);
		void remove(Collider* collider);
		void remove(RigidBody* collider);
		void update(sf::Time delta);
		void fixedUpdate();
		void findCollisions();
		void resolveCollisions();
		void updateRigidbodies();

	private:
		std::vector<Collider*> mColliders;
		std::vector<RigidBody*> mRigidBodies;
		std::vector<Manifold> mManifolds;
		sf::Time mStep = sf::seconds(1 / 100.f);
	};
}
