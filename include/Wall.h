#include "Component.h"

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}
}

class Wall : public hfm::Component
{
public:
	Wall(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, hfm::GameObject* go);
};