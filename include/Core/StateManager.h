#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <map>
#include <string>
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class State;

	class StateManager
	{
	public:
		static StateManager& getInstance();
		void registerState(std::string key, State* state);
		void setState(std::string stateName);
		std::map<std::string, State*>& getStates();
		State* getCurrentState();
		void setWindow(std::shared_ptr<sf::RenderWindow> window);
		void onEvent(sf::Event event);

	private:
		std::map<std::string, State*> mStates;
		State* mCurrentState = nullptr;
		std::shared_ptr<sf::RenderWindow> mWindow;
	};
}

