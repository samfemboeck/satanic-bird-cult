#include "Drawable.h"
#include "RenderManager.h"
#include "TiledMap.h"
#include "GameObject.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

hfm::Drawable::Drawable(const Drawable& other) : Component(other)
{
	mIsVisible = other.mIsVisible;
	mLayer = other.mLayer;
	mOffset = other.mOffset;
}

Drawable::Drawable(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Component(obj, group, go)
{
	std::map<std::string, std::string>::iterator it;
	if ((it = obj->Properties.find("drawLayer")) != obj->Properties.end())
	{
		setLayer(it->second);
	}
	
	if ((it = obj->Properties.find("isVisible")) != obj->Properties.end())
	{
		bool isVisible;
		std::istringstream(obj->Properties["isVisible"]) >> std::boolalpha >> isVisible;
		setVisible(isVisible);
	}
}

void hfm::Drawable::draw(sf::RenderWindow& window)
{
}

void Drawable::onDisable()
{
	RenderManager::getInstance().remove(this);
}

void Drawable::onEnable()
{
	RenderManager::getInstance().add(this);
}

std::string Drawable::getLayer()
{
	return mLayer;
}

void Drawable::setLayer(std::string layer)
{
	mLayer = layer;
}

void hfm::Drawable::setOffset(sf::Vector2f offset)
{
	mOffset = offset;
}

sf::Vector2f hfm::Drawable::getOffset()
{
	return mOffset;
}

void Drawable::setVisible(bool isVisible)
{
	mIsVisible = isVisible;
}

bool Drawable::isVisible()
{
	return mIsVisible;
}
