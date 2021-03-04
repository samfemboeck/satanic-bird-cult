#include "Pentagram.h"
#include "GameObjectManager.h"
#include "Map.h"
#include "Debug.h"
#include "GameObject.h"
#include "QuickMaths.h"
#include "RigidBody.h"
#include "Component.h"
#include "CircleCollider.h"
#include "TiledMap.h"

using namespace hfm;

Pentagram::Pentagram(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
{
}

void Pentagram::onStart()
{
	mRb = mGo->getComponent<RigidBody>();

	for (int i = 1; i <= 6; ++i)
	{
		auto point = GameObjectManager::getInstance().getObject("Pentagram Point " + std::to_string(i));
		mPoints.push_back(point->getPosition()); 
	}

	mGo->setEnabled(false);
}

void Pentagram::update(sf::Time delta)
{
	auto distance = *mTargetPoint - mGo->getPosition();
	const float threshold = 3;

	if (QuickMaths::length(distance) - threshold <= 0)
	{
		if (mActiveIdx == mIndexTraversal.size() - 1) // done with traversal
		{
			mGo->setEnabled(false);
			return;
		}

		mGo->setPosition(*mTargetPoint);
		mTargetPoint = &mPoints[mIndexTraversal[++mActiveIdx]];
		mRb->setVelocity(QuickMaths::normalize(*mTargetPoint - mGo->getPosition()) * mSpeed);
	}
}

void Pentagram::traverse(const std::vector<int>& traversal, float speed)
{
	mSpeed = speed;
	mGo->setEnabled(true);
	mIndexTraversal = traversal;
	mActiveIdx = 0;
	mTargetPoint = &mPoints[mIndexTraversal[mActiveIdx]];
	mGo->setPosition(*mTargetPoint);
}
