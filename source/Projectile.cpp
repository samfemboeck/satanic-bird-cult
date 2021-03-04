#include "Projectile.h"
#include "GameObject.h"
#include "PolygonCollider.h"
#include <cmath>
#include "Map.h"
#include "QuickMaths.h"
#include "RigidBody.h"
#include "Bird.h"
#include "CircleCollider.h"
#include "Player.h"
#include "TimerManager.h"
#include "BirdWall.h"
#include "ProjectileWall.h"
#include "NoPointWall.h"
#include "SoundManager.h"
#include "StateManager.h"
#include <iostream>
#include "TimerManager.h"
#include "MainState.h"
# define M_PI           3.14159265358979323846  /* pi */

using namespace hfm;

Projectile::Projectile()
{
}

Projectile::Projectile(sf::Vector2f& shootingDirection, sf::Vector2f startPos)
{
	mShootingDirection = shootingDirection;
	mStartPos = startPos;
}

Projectile::Projectile(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go) : Component(obj, group, go)
{
}

void Projectile::onStart()
{
	mRb = mGo->getComponent<RigidBody>();
}

void Projectile::setPlayer(Player* player)
{
	mPlayer = player;
	mStartPos = player->getGameObject()->getPosition();
	mGo->setPosition(mStartPos);
}

Player* Projectile::getPlayer()
{
	return mPlayer;
}

void Projectile::onCollision(Collider* other)
{
	if (!isEnabled())
		return;	

	auto spawn = other->getGameObject()->getName() == "Spawn";

	if (spawn)
	{
		mGo->markForDelete();
	}

	auto nopointwall = other->getGameObject()->getName() == "No Point Wall" || other->getGameObject()->getName() == "Bird Wall";

	if (nopointwall)
	{
		auto gameState = dynamic_cast<MainState*>(StateManager::getInstance().getCurrentState());
		if (gameState->checkEndState())
			return;

		mPlayer->addPoints(mValue);
		auto player = mPlayer;
		auto value = mValue;
		auto center = mGo->getPosition();

		TimerManager::getInstance().create(sf::seconds(0), [player, value, center, gameState] {
			gameState->displayPlusPoints(player, value, center);
			});

		mGo->markForDelete();
		mRb->setEnabled(false);
		setEnabled(false);
	}

	auto bird = other->getGameObject()->getComponent<Bird>();

	if (bird && !bird->isShot())
	{
		bird->changeValue(mBirdCounter++);
	}

	if (bird)
	{
		bird->getGameObject()->getComponent<CircleCollider>()->setTrigger(true);
		bird->shoot(mRb->getVelocity());
		bird->setLastHitProjectile(this, mStartPos);
	}

	auto player = other->getGameObject()->getComponent<Player>();

	if (player)
	{
		player->getGameObject()->getComponent<RigidBody>()->addImpulse(mShootingDirection * 100000.f);
	}
}

void Projectile::setShootingDirection(sf::Vector2f& dir)
{
	mShootingDirection = dir;
	mRb->setVelocity(dir * 700.0f);
	float angle = QuickMaths::axisOrientation(dir);
	mGo->setRotation(angle - 90);
}

void Projectile::update(sf::Time delta)
{
	if (QuickMaths::length(mGo->getPosition() - mStartPos) > 550)
	{
		mGo->markForDelete();
	}
}

void Projectile::addValue(int value)
{
	mValue += value;
}
