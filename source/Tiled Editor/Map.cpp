#include "Map.h"
#include "RigidBody.h"
#include "GameObject.h"
#include <random>
#include "TiledMap.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;
		
float Map::Scale = 1.f;

Map::Map(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Component(obj, group, go)
{
}

Map* hfm::Map::clone()
{
	return new Map(*this);
}
