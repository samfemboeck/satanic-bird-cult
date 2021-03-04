#include "BirdWall.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "TiledMap.h"

using namespace hfm;

BirdWall::BirdWall(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
{
}

void BirdWall::onStart()
{
	auto rb = mGo->getComponent<RigidBody>();
	// TODO
	//rb->setKinematic(true);
	rb->setIgnore("playerwall", true);
}
