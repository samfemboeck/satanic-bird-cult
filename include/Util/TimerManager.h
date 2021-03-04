#pragma once
#include "Timer.h"
#include <memory>
#include <list>
#include <vector>

namespace hfm
{
	class TimerManager
	{
	public:
		static TimerManager& getInstance();
		void update();
		void create(sf::Time duration, std::function<void()> callback);
		void clear();

	private:
		std::vector<Timer*> mToAdd;
		std::vector<std::unique_ptr<Timer>> mTimers;
	};
}

