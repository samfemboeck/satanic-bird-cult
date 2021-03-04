#include "Player.h"
#include "Gamepad.h"
#include "InputManager.h"
#include <iostream>
#include "AnimationRenderer.h"
#include "GameObject.h"
#include "QuickMaths.h"
#include "RigidBody.h"
#include "Projectile.h"
#include "GameObjectManager.h"
#include "Map.h"
#include "Collider.h"
#include "AABBCollider.h"
#include "Pentagram.h"
#include "SoundManager.h"

using namespace hfm;

Player::~Player()
{
}

Player::Player(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
{
}

void Player::update(sf::Time delta)
{	
	if (!mGamepad)
		return;

	mProjectileElapsed += delta;

	// dash (not hacky)
	if (mGamepad && mGamepad->isButtonDown(Gamepad::Button::LTrigger) && mDashCoolDown < 1)
	{
		auto color = mAnim->getSprite().getColor();
		mAnim->getSprite().setColor(sf::Color(color.r, color.g, color.b, 120));
		mDashCoolDown = 5.0f;
		mDashPhase1 = true;
		SoundManager::getInstance().play("dash");
	}

	// move
	if (!mDashPhase1 && !mDashPhase2)
	{
		mMove = { mGamepad->getAxis(Gamepad::Axis::LStickHorizontal), mGamepad->getAxis(Gamepad::Axis::LStickVertical) };
	}

	// shoot 
	sf::Vector2f shootDirection(mGamepad->getAxis(Gamepad::Axis::RStickHorizontal), mGamepad->getAxis(Gamepad::Axis::RStickVertical));
	if (mGamepad && mGamepad->isButtonDown(Gamepad::Button::RTrigger) && mProjectileElapsed >= mProjectileCooldown)
	{
		mProjectileElapsed = sf::seconds(0);
		sf::Vector2f normalizeDirection = QuickMaths::normalize(shootDirection);
		if (normalizeDirection.x != 0 || normalizeDirection.y != 0)
		{
			static int projectiles = 0;
			GameObject* projGo = GameObjectManager::getInstance().cloneTemplate("Projectile", "Projectile " + std::to_string(mGamepad->getIdx()) + std::to_string(projectiles++));
			Projectile* proj = projGo->getComponent<Projectile>();
			proj->setPlayer(this);
			proj->setShootingDirection(normalizeDirection);
			mRigidBody->setIgnore(projGo->getComponent<RigidBody>(), true);
			proj->setEnabled(true);

			SoundManager::getInstance().play("knife");
		}
	}

	if (shootDirection.x != 0 || shootDirection.y != 0)
	{
		// angle
		float aimAngle = QuickMaths::axisOrientation(shootDirection);
		mAim->setRotation(aimAngle+180);
		auto renderer = mAim->getComponent<SpriteRenderer>();
		if (mProjectileElapsed.asSeconds() > 0.5)
		{
			renderer->getSprite().setColor(sf::Color(0,138,255, 100));
		}
		else {
			renderer->getSprite().setColor(sf::Color(255, 255, 255, 30));
		}
	}
	else {
		auto renderer = mAim->getComponent<SpriteRenderer>();
		renderer->getSprite().setColor(sf::Color(0, 138, 255, 0));
	}
	
	
	if (mMove.x == 0 && mMove.y == 0)
	{
		std::string currentAnim = mAnim->getActiveAnimation();
		if (currentAnim == "walk_left")
			mAnim->play("idle_left");
		else if (currentAnim == "walk_north")
			mAnim->play("idle_north");
		else if (currentAnim == "walk_right")
			mAnim->play("idle_right");
		else if (currentAnim == "walk_south")
			mAnim->play("idle_south");
	}
	else 
	{
		float angle = QuickMaths::axisOrientation(mMove);
		if (angle > 315 || angle <= 45)
			mAnim->play("walk_left");
		else if (angle > 45 && angle <= 135)
			mAnim->play("walk_north");
		else if (angle > 135 && angle <= 225)
			mAnim->play("walk_right");
		else
			mAnim->play("walk_south");
	}
}

void Player::physicsUpdate(sf::Time delta)
{
	if (mDashCoolDown > 1)
	{
		mDashCoolDown -= 0.04f;	
	}

	// dash
	if (mDashPhase1)
	{
		mSpeed += 0.15;
		mRigidBody->setIgnore("bird", true);
		mRigidBody->setIgnore("player", true);
	}
	if (mSpeed > 4.5)
	{
		mDashPhase1 = false;
		mDashPhase2 = true;
	}
	if (mDashPhase2)
	{
		mSpeed -= 0.20;
	}
	if (mSpeed < 1)
	{
		mRigidBody->setIgnore("bird", false);
		mRigidBody->setIgnore("player", false);
		mDashPhase2 = false;
		auto color = mAnim->getSprite().getColor();
		mAnim->getSprite().setColor(sf::Color(color.r, color.g, color.b, 255));
		mSpeed = 1;
	}

	if (mMove.x != 0 || mMove.y != 0)
	{
		mRigidBody->addImpulse(QuickMaths::normalize(mMove) * mSpeed * 2000.f);
		mLastMove = mMove;
	}
	else if ((mLastMove.x != 0 || mLastMove.y != 0) && mSpeed > 1.15)
	{
		mRigidBody->addImpulse(QuickMaths::normalize(mLastMove) * mSpeed * 2000.f);
	}

	mRigidBody->setVelocity(mRigidBody->getVelocity() * 0.91f);
}

void Player::onStart()
{
	mRigidBody = mGo->getComponent<RigidBody>();
	mAnim = mGo->getComponent<AnimationRenderer>();
}

void Player::onAdd(hfm::GameObject* go)
{
	Component::onAdd(go);

	// TODO implement setColor() method for player animation/aim indicator
	/*
	if (mGamepad->getIdx() == 0)
	{
	}
	else if (mGamepad->getIdx() == 1)
	{
		auto aimSprite1 = std::make_unique<SpriteRenderer>("Textures/aimgreen1.png");
		aimSprite1->setScale(Map::Scale, Map::Scale);
		aimSprite1->setLayer("aimIndicator");
		aimSprite1->setOrigin(8, 8);
		aimSprite1->setColor(sf::Color(aimSprite1->getColor().r, aimSprite1->getColor().g, aimSprite1->getColor().b, 0));
		go->addComponent(std::move(aimSprite1));
	}
	*/
}

void Player::addPoints(int points)
{
	mPoints += points;
}

int Player::getPoints()
{
	return mPoints;
}

Gamepad* Player::getGamepad()
{
	return mGamepad;
}

void Player::setGamepad(hfm::Gamepad* gamepad)
{
	mGamepad = gamepad;
	mAim = GameObjectManager::getInstance().cloneTemplate("Aim", "Aim " + std::to_string(mGamepad->getIdx()));
	mAim->setPosition(mGo->getPosition());
	mAim->setParent(mGo);
}

void Player::onCollision(Collider* other)
{
}

sf::Time Player::getProjectileCooldown()
{
	return mProjectileCooldown;
}
