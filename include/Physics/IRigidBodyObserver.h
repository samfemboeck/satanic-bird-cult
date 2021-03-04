#pragma once

namespace hfm
{
	class Collider;

	class IRigidBodyObserver
	{
	public:
		virtual void onCollision(hfm::Collider* other) = 0;
	};
}