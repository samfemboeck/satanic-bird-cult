#pragma once
#include "State.h"
#include <SFML/Audio.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class Gamepad;
	class AnimationRenderer;
}
	
class Player;

class MainState : public hfm::State
{
public:
	struct PlusPoint
	{
		sf::Time showInterval = sf::seconds(1);
		sf::Clock clock;
		bool isVisible = false;
		tgui::Label::Ptr label;
		Player* player;
	};

	void init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;
	void displayPlusPoints(Player* player, int points, sf::Vector2f position);
	bool checkEndState();

private:
	sf::RenderWindow* mWindow;
	hfm::Gamepad* mGamepad = nullptr;
	sf::Sound* mSound = nullptr;
	sf::SoundBuffer* mSoundBuffer = nullptr;
	float mBirdSpawnTime = 0.0f;
	tgui::TextBox::Ptr mP1Points;
	hfm::AnimationRenderer* mPlayer1Idle = nullptr;
	hfm::AnimationRenderer* mPlayer2Idle = nullptr;
	float mGameTimer = 5.0f;
	float mEndTimer = 3.0f;
	bool mIsEndTime = false;
	float mBirdSpawnCooldown = 3;
	std::map<Player*, PlusPoint> mPlusPoints;
	unsigned long int mBirdCounter = 0;
};
