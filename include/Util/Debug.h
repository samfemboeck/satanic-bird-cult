#include <SFML/System/Vector2.hpp>
#include <string>
// (c) Coding: Femb�ck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class Debug
	{
	public:
		static void log(std::string Name, sf::Vector2f);
	};
}