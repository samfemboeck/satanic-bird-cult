#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "InputManager.h"
#include <iostream>
#include "GameObjectManager.h"
#include "ContentManager.h"
#include "RenderManager.h"
#include "StateManager.h"
#include "State.h"
#include "PhysicsManager.h"
#include "SoundManager.h"
#include <TGUI/TGUI.hpp>
#include "TimerManager.h"
// (c) Coding: Femb?ck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

void Game::run()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	while (mWindow->isOpen())
	{
		sf::Event event;

		while (mWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				mWindow->close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				InputManager::getInstance().onKeyPressed(event.key.code);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				InputManager::getInstance().onKeyReleased(event.key.code);
			}

			StateManager::getInstance().onEvent(event);
		}

		update();
		draw();
	}
   
    shutdown();
    _CrtDumpMemoryLeaks();
}

bool Game::init()
{
    mWindow = std::make_shared<sf::RenderWindow>();
    mWindow->create(sf::VideoMode(1920, 1080), "Space Shooter"/*, sf::Style::Fullscreen*/); //sf::Style::Fullscreen

    StateManager::getInstance().setWindow(mWindow);
      
    return true;
}

void Game::update()
{
    if (InputManager::getInstance().isKeyDown(sf::Keyboard::Escape))
    {
        mWindow->close();
    }

	static sf::Clock clock;
	sf::Time delta = clock.restart();
    mElapsed += delta;
    ++mFrames;

    if (mElapsed > sf::seconds(0.5))
    {
		mWindow->setTitle(std::to_string(mFrames) + " FPS");
        mElapsed = sf::seconds(0);
        mFrames = 0;
    }

    SoundManager::getInstance().update();
    TimerManager::getInstance().update();
    InputManager::getInstance().update();
    StateManager::getInstance().getCurrentState()->update(delta);
    GameObjectManager::getInstance().update(delta);
    InputManager::getInstance().postUpdate();
    PhysicsManager::getInstance().update(delta);
}

void Game::draw()
{
    mWindow->clear(sf::Color(0, 0, 0));
    RenderManager::getInstance().draw(*mWindow);
    StateManager::getInstance().getCurrentState()->draw(*mWindow);
    mWindow->display();
}

void Game::shutdown()
{
    StateManager::getInstance().getCurrentState()->exit();
    for (auto state : StateManager::getInstance().getStates())
    {
        delete state.second;
        state.second = nullptr;
    }

    GameObjectManager::getInstance().clear();
}
