#include "RigidBody.h"
#include <iostream>
#include "GameObject.h"
#include "IRigidBodyObserver.h"
#include "PhysicsManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

RigidBody::RigidBody(const RigidBody& other)
{
	mInvMass = other.mInvMass;
	mAcceleration = other.mAcceleration;
	mVelocity = other.mVelocity;
	mForces = other.mForces;
	mImpulses = other.mImpulses;
	mLayer = other.mLayer;
	mIgnoredLayers = other.mIgnoredLayers;
	mIgnoredRbs = other.mIgnoredRbs;
}

RigidBody::RigidBody(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Component(obj, group, go)
{
	std::map<string, string>::iterator it;
	if ((it = obj->Properties.find("physicsLayer")) != obj->Properties.end())
	{
		setLayer(it->second);
	}
	
	if ((it = obj->Properties.find("mass")) != obj->Properties.end())
	{
		setInvMass(1.f / std::stof(it->second));
	}

	if ((it = obj->Properties.find("isKinematic")) != obj->Properties.end())
	{
		bool isKinematic;
		std::istringstream(obj->Properties["isKinematic"]) >> std::boolalpha >> isKinematic;
		if (isKinematic)
			mInvMass = 0;
	}

	if ((it = obj->Properties.find("ignore")) != obj->Properties.end())
	{
		std::stringstream test(obj->Properties["ignore"]);
		std::string segment;
		std::vector<std::string> seglist;

		while (std::getline(test, segment, ','))
		{
			setIgnore(segment, true);
		}
	}
}

RigidBody::~RigidBody()
{
}

RigidBody* RigidBody::clone()
{
	return new RigidBody(*this);
}

void RigidBody::onAwake()
{
}

void RigidBody::onStart()
{
	mObservers = mGo->getComponents<IRigidBodyObserver>();
}

void RigidBody::onEnable()
{
	PhysicsManager::getInstance().add(this);
}

void RigidBody::onDisable()
{
	PhysicsManager::getInstance().remove(this);
}

void RigidBody::correctPosition(sf::Time delta)
{
	mGo->move(mVelocity * delta.asSeconds() + mCorrection);
	mCorrection.x = mCorrection.y = 0;
}

void RigidBody::onCollision(Collider* other)
{
	for (auto observer : mObservers)
		observer->onCollision(other);
}

float RigidBody::getInvMass()
{
	return mInvMass;
}

void RigidBody::setInvMass(float Value)
{
	mInvMass = Value;
}

sf::Vector2f RigidBody::getAcceleration()
{
	return mAcceleration;
} 

void RigidBody::setAcceleration(sf::Vector2f acc)
{
	mAcceleration = acc;
}

sf::Vector2f RigidBody::getVelocity()
{
	return mVelocity;
}

void RigidBody::setVelocity(sf::Vector2f vel)
{
	mVelocity = vel;
}
  
void RigidBody::addForce(sf::Vector2f force)
{
	mForces.push_back(force);
}

void RigidBody::addImpulse(sf::Vector2f impulse)
{
	mImpulses.push_back(impulse);
}

std::list<sf::Vector2f>& RigidBody::getForces()
{
	return mForces;
}

std::list<sf::Vector2f>& RigidBody::getImpulses()
{
	return mImpulses;
}

void RigidBody::setIgnore(std::string layer, bool ignore)
{
	mIgnoredLayers[layer] = ignore;
}

void RigidBody::setIgnore(RigidBody* rb, bool ignore)
{
	mIgnoredRbs[rb] = ignore;
}

bool RigidBody::hasIgnore(RigidBody* rb)
{
	auto it = mIgnoredRbs.find(rb);
	if (it != mIgnoredRbs.end() && mIgnoredRbs[rb])
		return true;

	auto it2 = mIgnoredLayers.find(rb->getLayer());
	if (it2 != mIgnoredLayers.end() && mIgnoredLayers[rb->getLayer()])
		return true;

	return false;
}

std::string RigidBody::getLayer()
{
	return mLayer;
}

void RigidBody::setLayer(std::string layer)
{
	mLayer = layer;
}

void RigidBody::addCorrection(const sf::Vector2f& correction)
{
	mCorrection += correction;
}
