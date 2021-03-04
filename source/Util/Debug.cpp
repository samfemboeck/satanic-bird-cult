#include "Debug.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

void Debug::log(std::string Name, sf::Vector2f vec)
{
	std::cout << "<" << Name << "> x: " << vec.x << ", y: " << vec.y << std::endl;
}
