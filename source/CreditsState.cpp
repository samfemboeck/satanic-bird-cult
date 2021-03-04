#include "CreditsState.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "TimerManager.h"
#include "ContentManager.h"
#include "StateManager.h"

using namespace hfm;

void CreditsState::init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window)
{
	State::init(manager, window);
    ContentManager::getInstance().loadMap("../assets/credits.tmx", {});
	TimerManager::getInstance().create(sf::seconds(2), [this]() { StateManager::getInstance().setState("menu"); });
}

void CreditsState::exit()
{
    GameObjectManager::getInstance().clear();
}