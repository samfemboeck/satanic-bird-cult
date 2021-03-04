#include "IntroState.h"
#include "Game.h"
#include "SpriteRenderer.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include <iostream>
#include <TGUI/TGUI.hpp>
#include "TimerManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "ContentManager.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

void IntroState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
    State::init(manager, window);

    ContentManager::getInstance().loadMap("../assets/intro.tmx", {});
	TimerManager::getInstance().create(sf::seconds(2), []() { StateManager::getInstance().setState("credits"); });
}

void IntroState::exit()
{
    GameObjectManager::getInstance().clear();
}