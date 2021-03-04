#pragma once
#include "State.h"

class CreditsState : public hfm::State
{
public:
	CreditsState() = default;
	void init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
};

