#include "Timer.h"
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

Timer::Timer(sf::Time duration, std::function<void()> callback)
{
	mDuration = duration;
	mStartTime = mClock.restart();
	mCallback = callback;
}

bool Timer::isFinished()
{
	return mClock.getElapsedTime() - mStartTime >= mDuration;
}

std::function<void()> Timer::getCallBack()
{
	return mCallback;
}

void Timer::setTriggered(bool isTriggered)
{
	mWasTriggered = isTriggered;
}

bool Timer::wasTriggered()
{
	return mWasTriggered;
}
