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

class ProjectileWall : public hfm::Component
{
public:
	ProjectileWall(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);
	void onStart() override;
};

