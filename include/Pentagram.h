#pragma once
#include "Component.h"

namespace hfm
{
	class RigidBody;
	
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}
}

class Pentagram : public hfm::Component
{
public:
	Pentagram(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);
	void onStart() override;
	void update(sf::Time delta) override;
	void traverse(const std::vector<int>& traversal, float speed);

private:
	std::vector<sf::Vector2f> mPoints;
	std::vector<int> mIndexTraversal;
	sf::Vector2f* mTargetPoint = nullptr;
	int mActiveIdx = 0;
	float mSpeed = 0.0f;

	hfm::RigidBody* mRb = nullptr;
};

