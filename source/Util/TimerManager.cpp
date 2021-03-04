#include "TimerManager.h"
#include <iostream>
#include "StateManager.h"

using namespace hfm;

TimerManager& TimerManager::getInstance()
{
	static TimerManager instance;
	return instance;
}

void TimerManager::update()
{
	for (auto t : mToAdd)
		mTimers.push_back(std::unique_ptr<Timer>(t));
	mToAdd.clear();

	std::vector<std::unique_ptr<Timer>>::iterator it;
	for (it = mTimers.begin(); it != mTimers.end();)
	{
		if ((*it)->isFinished())
		{
			(*it)->getCallBack()();
			it = mTimers.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void TimerManager::create(sf::Time duration, std::function<void()> callback)
{
	mToAdd.push_back(new Timer(duration, callback));
}

void TimerManager::clear()
{
	mTimers.clear();
}
