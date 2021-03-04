#pragma once
#include "Component.h"
#include "IRigidBodyObserver.h"

namespace hfm
{
	class Gamepad;
	class AnimationRenderer;
	class RigidBody;

	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}
}

class Player : public hfm::Component, public hfm::IRigidBodyObserver
{
public:
	~Player() override;
	Player() = default;
	Player(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);

	virtual Player* clone() { return new Player(*this); }
	virtual void update(sf::Time delta) override;
	virtual void physicsUpdate(sf::Time delta) override;
	virtual void onStart() override;
	virtual void onAdd(hfm::GameObject* go) override;
	virtual void onCollision(hfm::Collider* other) override;

	void addPoints(int points);
	int getPoints();
	hfm::Gamepad* getGamepad();
	void setGamepad(hfm::Gamepad*);
	sf::Time getProjectileCooldown();

private:
	float mSpeed = 1;
	hfm::Gamepad* mGamepad = nullptr;
	hfm::AnimationRenderer* mAnim = nullptr;
	sf::Vector2f mShootingDirection;
	sf::Vector2f mMove;
	hfm::RigidBody* mRigidBody = nullptr;
	hfm::GameObject* mAim;
	int mPoints = 0;
	bool mDashPhase1 = false;
	bool mDashPhase2 = false;
	float mDashCoolDown = 0;
	sf::Time mProjectileElapsed = sf::seconds(0);
	sf::Time mProjectileCooldown = sf::seconds(0.5);
	sf::Vector2f mLastMove = sf::Vector2f(0, 1);
};

