#include "Bird.h"
#include "CircleCollider.h"
#include "QuickMaths.h"
#include "GameObjectManager.h"
#include "Debug.h"
#include "AnimationRenderer.h"
#include "Wall.h"
#include "Projectile.h"
#include "AABBCollider.h"
#include "Player.h"
#include <iostream>
#include "TimerManager.h"
#include "Pentagram.h"
#include "Projectile.h"
#include "NoPointWall.h"
#include "SoundManager.h"
#include "RigidBody.h"

using namespace hfm;

int Bird::mInstances = 0;

Bird::Bird()
{
	mInstances++;
}

Bird::Bird(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go) : Component(obj, group, go)
{
}

Bird::~Bird()
{
	mInstances--;
}

void Bird::onStart()
{
	mRb = mGo->getComponent<RigidBody>();
	mAnim = mGo->getComponent<AnimationRenderer>();
	newDestination();
}

void Bird::update(sf::Time delta)
{
	mDirection = sf::Vector2f(mDestination - mGo->getPosition());

	// update animation
	float angle = QuickMaths::axisOrientation(mDirection);
	if (angle > 315 || angle <= 45)
		mAnim->play("walk_left");
	else if (angle > 45 && angle <= 135)
		mAnim->play("walk_north");
	else if (angle > 135 && angle <= 225)
		mAnim->play("walk_right");
	else
		mAnim->play("walk_south");

	// update destination

	if (mFollow) // bird follows somebody
	{
		mDestination = mFollowObject->getPosition();
	}
	else // does not follow somebody
	{
		mNewPosTimer += delta.asSeconds();
		if (mNewPosTimer >= 10.0f)
		{
			newDestination();
			mNewPosTimer = 0.0f;
		}

		// check distance to players
		auto player1 = GameObjectManager::getInstance().getObject("Player 1");
		auto player2 = GameObjectManager::getInstance().getObject("Player 2");
		auto posPlayer1 = player1->getPosition();
		auto posPlayer2 = player2->getPosition();
		auto posBird = mGo->getPosition();
		auto distanceP1 = QuickMaths::length(posPlayer1 - posBird);
		auto distanceP2 = QuickMaths::length(posPlayer2 - posBird);
		if (distanceP1 < distanceP2 && distanceP1 < 130)
		{
			mFollowObject = player1;
			mFollow = true;
		}
		else if (distanceP2 < 130)
		{
			mFollowObject = player2;
			mFollow = true;
		}
	}

	if (!mFollow)
	{
		if (abs(mGo->getPosition().x - mDestination.x) <= 5 && abs(mGo->getPosition().y - mDestination.y) <= 5)
		{
			newDestination();
		}
	}

	if (mWasShot && QuickMaths::length(mGo->getPosition() - mProjectileStart) > 550)
	{
		mGo->markForDelete();
	}
}

void Bird::physicsUpdate(sf::Time delta)
{
	if (!mWasShot)
	{
		if (mFollow)
		{
			if (QuickMaths::length(mGo->getPosition() - mDestination) > mMinPlayerDistance)
				mGo->getComponent<RigidBody>()->setVelocity(QuickMaths::normalize(mDirection) * 150.f);
			else
				mGo->getComponent<RigidBody>()->setVelocity({0, 0});
		}
		else
		{
			mGo->getComponent<RigidBody>()->setVelocity(QuickMaths::normalize(mDirection) * 150.f);
		}
	}
}

void Bird::setLastHitProjectile(Projectile* projectile, sf::Vector2f projStartPos)
{
	mLastHitProjectile = projectile;
	mProjectileStart = projStartPos;
}

void Bird::newDestination()
{
	mDestination = QuickMaths::newRandomPosition(sf::Vector2f(200,200),sf::Vector2f(1720,880));
}

void Bird::shoot(sf::Vector2f force)
{
	mWasShot = true;
	mRb->setVelocity(force);
}

void Bird::onCollision(Collider* other)
{
	if (!isEnabled())
		return;

	auto spawn = other->getGameObject()->getName() == "Spawn";

	if (spawn)
	{
		mGo->getComponent<CircleCollider>()->setTrigger(true);
	}
	else if (!mWasShot) {
		mGo->getComponent<CircleCollider>()->setTrigger(false);
	}

	auto map = other->getGameObject()->getName() == "Bird Wall";

	if (map && mWasShot)
	{
		mGivePoints = true;
		mAnim->play("blood-splatter");
		if (mIsInPentagram)
			mValue *= 3;

		mLastHitProjectile->addValue(mValue);

		std::string name = mGo->getName();
		TimerManager::getInstance().create(sf::seconds(1), [name]() 
		{
			auto go = GameObjectManager::getInstance().getObject(name);
			if (go)
			{
				go->markForDelete();
			}
		});

		SoundManager::getInstance().play("blood");

		mRb->setEnabled(false);
		setEnabled(false);
	}

	auto nopointwall = other->getGameObject()->getName() == "No Point Wall";

	if (nopointwall)
	{
		mGo->markForDelete();
		setEnabled(false);
	}

	mIsInPentagram = other->getGameObject()->getComponent<Pentagram>() != nullptr;
}

void Bird::changeValue(int value)
{
	mValue = value;
}

bool Bird::isShot()
{
	return mWasShot;
}
