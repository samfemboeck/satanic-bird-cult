#pragma once
#include "State.h"

class InfoState : public hfm::State
{
public:
	InfoState() = default;
	void init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;

private:
	bool mInfoBuffer = false;

};