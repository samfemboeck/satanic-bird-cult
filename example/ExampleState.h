#pragma once
#include <State.h>

class ExampleState : public hfm::State
{
	virtual void init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window);
	virtual void exit();
	virtual void update(sf::Time delta);
};
