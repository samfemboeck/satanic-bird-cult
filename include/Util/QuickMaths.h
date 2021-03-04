#pragma once
#include <SFML/System/Vector2.hpp>
# define M_PI           3.14159265358979323846  /* pi */
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class QuickMaths
	{
	public:
		static sf::Vector2f normalize(const sf::Vector2f& vec);
		static float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
		static float length(const sf::Vector2f& vec);
		static sf::Vector2f perp(const sf::Vector2f& vec);
		static sf::Vector2f negate(const sf::Vector2f& vec);
		static float axisOrientation(const sf::Vector2f& vec);
		static sf::Vector2f newRandomPosition(const sf::Vector2f startCorner, const sf::Vector2f endCorner);
	};
}

