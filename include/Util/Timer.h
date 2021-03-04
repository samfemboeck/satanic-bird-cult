#pragma once
#include <functional>
#include <SFML/System/Clock.hpp>

namespace hfm
{
	class Timer
	{
	public:
		Timer(sf::Time duration, std::function<void()> callback);
		bool isFinished();
		std::function<void()> getCallBack();
		void setTriggered(bool isTriggered);
		bool wasTriggered();

	private:
		bool mWasTriggered = false;
		sf::Clock mClock;
		sf::Time mStartTime;
		sf::Time mDuration;
		std::function<void()> mCallback;
	};
}
