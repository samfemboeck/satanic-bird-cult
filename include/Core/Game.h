#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <vector>
#include <TGUI/TGUI.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	typedef std::shared_ptr<sf::Sprite> SpritePtr;

	class Game
	{
	public:
		void run();
		bool init();

	private:
		void update();
		void draw();
		void shutdown();

		std::shared_ptr<sf::RenderWindow> mWindow;
		sf::Time mElapsed;
		int mFrames = 0;
	};
}