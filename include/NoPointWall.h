#pragma once
#include "Component.h"

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}
}

class NoPointWall : public hfm::Component
{
public:
	NoPointWall(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);
	void onStart() override;
};