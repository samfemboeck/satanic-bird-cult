#include "EndState.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "AnimationRenderer.h"
#include <RigidBody.h>
#include <Map.h>
#include "StateManager.h"
#include "ContentManager.h"

using namespace hfm;

void EndState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);

    // who won
    tgui::Label::Ptr winner = tgui::Label::create();
    winner->setText(mWinnner + " won, with " + std::to_string(mWinnerPoints) + " Points!");
    winner->setTextSize(60);
    winner->setPosition(sf::Vector2f(960,750) - (0.5f * winner->getFullSize()));
    winner->getRenderer()->setTextColor(tgui::Color(255, 255, 255, 255));
    mGui.add(winner, "Winner");

    Map::Scale = 1;
    ContentManager::getInstance().loadMap("../assets/end.tmx", {});
}

void EndState::exit()
{
    mWinnerPoints = 0;
    mGui.removeAllWidgets();
    InputManager::getInstance().clear();
    GameObjectManager::getInstance().clear();
}

void EndState::update(sf::Time delta)
{
    if (InputManager::getInstance().isKeyDown(sf::Keyboard::Space))
    {
        StateManager::getInstance().setState("menu");
    }

    if ((sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 0))
        || (sf::Joystick::isConnected(0) && sf::Joystick::isButtonPressed(0, 1))
        || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 0))
        || (sf::Joystick::isConnected(1) && sf::Joystick::isButtonPressed(1, 1)))
    {
        StateManager::getInstance().setState("menu");
    }
}

void EndState::setWinner(std::string winner, int points)
{
    mWinnner = winner;
    mWinnerPoints = points;
}
