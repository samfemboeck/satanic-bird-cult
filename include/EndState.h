#pragma once
#include "State.h"

class EndState : public hfm::State
{
public:
	EndState() = default;
	void init(hfm::StateManager* manager, std::shared_ptr<sf::RenderWindow> window) override;
	void exit() override;
	void update(sf::Time delta) override;
	void setWinner(std::string winner, int points);

private:
	std::string mWinnner;
	int mWinnerPoints = 0;
};