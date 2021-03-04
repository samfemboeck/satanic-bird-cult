#include "ProjectileWall.h"
#include "RigidBody.h"
#include "GameObject.h"

using namespace hfm;

ProjectileWall::ProjectileWall(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
{
}

void ProjectileWall::onStart()
{
	auto rb = mGo->getComponent<RigidBody>();
	rb->setInvMass(0);
	rb->setIgnore("birdWall", true);
}
