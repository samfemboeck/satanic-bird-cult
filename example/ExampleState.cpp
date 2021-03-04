#include "ExampleState.h"
#include <iostream>

void ExampleState::init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
	State::init(manager, window);
	std::cout << "Add TGUI Widgets, load .tmx map" << std::endl;
}

void ExampleState::update(sf::Time delta)
{
}

void ExampleState::exit()
{
	State::exit();
	std::cout << "Bye" << std::endl;
}
