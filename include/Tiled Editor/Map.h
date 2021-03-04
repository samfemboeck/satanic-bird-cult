#pragma once
#include "Component.h"
#include <SFML\System\Vector2.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}

	class Map : public Component
	{
	public:
		static float Scale;

		Map(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

		Map* clone() override;
	};
}

