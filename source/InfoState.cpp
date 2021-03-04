#include "InfoState.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "StateManager.h"
#include "ContentManager.h"

using namespace hfm;

void InfoState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);
    ContentManager::getInstance().loadMap("../assets/controls.tmx", {});
    TimerManager::getInstance().create(sf::seconds(2), [this]() {mInfoBuffer = true; });
}

void InfoState::exit()
{
    mInfoBuffer = false;
    GameObjectManager::getInstance().clear();
}

void InfoState::update(sf::Time delta)
{
    if (mInfoBuffer)
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
}
