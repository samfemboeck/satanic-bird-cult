#pragma once
#include <SFML/System/Vector2.hpp>
#include "Component.h"
#include "IRigidBodyObserver.h"

namespace hfm
{
	class RigidBody;
	class AnimationComponent;
	class GameObject;
	class Collider;

	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}
}

class Player;

class Projectile : public hfm::Component, public hfm::IRigidBodyObserver
{
public:
	Projectile();
	Projectile(sf::Vector2f& shootingDirection, sf::Vector2f startPos);
	Projectile(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);

	virtual Projectile* clone() override { return new Projectile(*this); };
	virtual void onStart() override;
	virtual void update(sf::Time delta) override;
	virtual void onCollision(hfm::Collider* other) override;

	void setPlayer(Player* player);
	Player* getPlayer();
	void setShootingDirection(sf::Vector2f& dir);
	void addValue(int value);
	
private:
	bool mIsDeleted = false;
	sf::Vector2f mStartPos;
	sf::Vector2f mShootingDirection;
	Player* mPlayer = nullptr;
	hfm::RigidBody* mRb = nullptr;
	hfm::AnimationComponent* mAnimBird = nullptr;
	int mValue = 0;
	int mBirdCounter = 1;
};