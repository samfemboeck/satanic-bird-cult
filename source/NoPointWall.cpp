#include "NoPointWall.h"
#include "GameObject.h"
#include "RigidBody.h"

using namespace hfm;

NoPointWall::NoPointWall(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
{
}

void NoPointWall::onStart()
{
	auto rb = mGo->getComponent<RigidBody>();
	// TODO
	rb->setIgnore("playerwall", true);
	//rb->setKinematic(true);
}