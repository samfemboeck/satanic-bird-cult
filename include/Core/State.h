#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <TGUI/TGUI.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class StateManager;

	class State
	{
	public:
		virtual void init(StateManager* manager, std::shared_ptr<sf::RenderWindow> window);
		virtual void exit();
		virtual void update(sf::Time delta);
		virtual void draw(sf::RenderWindow& window);

		void scaleGui(float scale);
		void onEvent(sf::Event& event);

	protected:
		StateManager* mManager;
		tgui::Gui mGui;
	};
}