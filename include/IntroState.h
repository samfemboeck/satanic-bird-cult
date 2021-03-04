#pragma once
#include "State.h"

class IntroState : public hfm::State
{
public:
	IntroState() = default;
	void init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
};

