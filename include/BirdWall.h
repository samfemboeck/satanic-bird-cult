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

class BirdWall : public hfm::Component
{
public:
	BirdWall(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);
	void onStart() override;
};

