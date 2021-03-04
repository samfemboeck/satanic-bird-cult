#include "State.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

void State::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    mGui.setTarget(*window);
	mManager = manager;
}

void hfm::State::exit()
{
	mGui.removeAllWidgets();
}

void hfm::State::update(sf::Time delta)
{
}

void State::draw(sf::RenderWindow& window)
{
	mGui.draw();
}

void hfm::State::scaleGui(float scale)
{
    for (const auto& widget : mGui.getWidgets())
    {
        widget->setSize(widget->getSize().x * scale, widget->getSize().y * scale);
        widget->setTextSize(widget->getTextSize() * scale);
    }
}

void State::onEvent(sf::Event& event)
{
	mGui.handleEvent(event);
}
