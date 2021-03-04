#pragma once
#include "Component.h"
#include "IRigidBodyObserver.h"

namespace hfm
{
	class AnimationRenderer;
	class RigidBody;

	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}
}

class Player;
class Projectile;

class Bird : public hfm::Component, public hfm::IRigidBodyObserver
{
public:
	static int mInstances;
	Bird();
	Bird(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);
	~Bird();

	virtual Bird* clone() { return new Bird(*this); }
	virtual void onStart() override;
	virtual void update(sf::Time delta) override;
	virtual void physicsUpdate(sf::Time delta) override;
	virtual void onCollision(hfm::Collider* other) override;

	void setLastHitProjectile(Projectile* projectile, sf::Vector2f projStartPos);
	void newDestination();
	void shoot(sf::Vector2f force);
	void changeValue(int value);
	bool isShot();

private:
	bool mFollow = false;
	bool mWasShot = false;
	bool mGivePoints = false;
	bool mIsInPentagram = false;
	float mNewPosTimer = 0.0f;
	bool mIsDeleted = false;
	float mMinPlayerDistance = 30;
	sf::Vector2f mDestination;
	sf::Vector2f mDirection;
	hfm::GameObject* mFollowObject = nullptr;
	hfm::AnimationRenderer* mAnim = nullptr;
	hfm::RigidBody* mRb = nullptr;
	int mValue = 1;
	sf::Vector2f mProjectileStart;
	Projectile* mLastHitProjectile = nullptr;
};