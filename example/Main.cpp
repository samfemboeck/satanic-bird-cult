#include <Game.h>
#include <StateManager.h>
#include "IntroState.h"
#include "CreditsState.h"
#include "MenuState.h"
#include "MainState.h"
#include "InfoState.h"
#include "EndState.h"
#include "ContentManager.h"
#include "Player.h"
#include "Bird.h"
#include "RenderManager.h"
#include "Projectile.h"
#include "Pentagram.h"

using namespace hfm;

int main()
{
	Game game;
	game.init();

    ContentManager::getInstance().addCreator("Player", creator<Player>);
    ContentManager::getInstance().addCreator("Bird", creator<Bird>);
    ContentManager::getInstance().addCreator("Projectile", creator<Projectile>);
    ContentManager::getInstance().addCreator("Pentagram", creator<Pentagram>);

	RenderManager::getInstance().addLayer("pentagram", 1);
	RenderManager::getInstance().addLayer("projectile", 2);
	RenderManager::getInstance().addLayer("aim", 3);
	RenderManager::getInstance().addLayer("shadow", 4);
	RenderManager::getInstance().addLayer("candles", 5);
	RenderManager::getInstance().addLayer("player", 6);
	RenderManager::getInstance().addLayer("bird", 7);

	StateManager::getInstance().registerState("intro", new IntroState);
	StateManager::getInstance().registerState("credits", new CreditsState);
	StateManager::getInstance().registerState("menu", new MenuState);
	StateManager::getInstance().registerState("info", new InfoState);
	StateManager::getInstance().registerState("main", new MainState);
	StateManager::getInstance().registerState("end", new EndState);
	StateManager::getInstance().setState("intro");

	game.run();
}