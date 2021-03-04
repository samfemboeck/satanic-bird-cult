#include "SpriteRenderer.h"
#include <iostream>
#include "GameObject.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <unordered_map>
#include "RigidBody.h"
#include "TiledMap.h"
#include "ContentManager.h"

using namespace hfm;

void SpriteRenderer::onStart()
{
}

void SpriteRenderer::onAwake()
{
	Drawable::onAwake();
}

void hfm::SpriteRenderer::draw(sf::RenderWindow& window)
{
	auto transform = mGo->getTransform();
	transform.translate(mOffset);
	window.draw(mSprite, transform);
}

hfm::SpriteRenderer::SpriteRenderer(const SpriteRenderer& other) : Drawable(other)
{
	mSprite = other.mSprite;
}

SpriteRenderer::SpriteRenderer(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Drawable(obj, group, go)
{
	auto tileset = ContentManager::getInstance().getMap().getTilesetForGid(obj->Gid);	
	mSprite = ContentManager::getInstance().getSpriteForGid(obj->Gid);	
    auto scale = Map::Scale * sf::Vector2f{ obj->Width / tileset->TileWidth, obj->Height / tileset->TileHeight };
	mSprite.setScale(scale);
	auto pos = Map::Scale * sf::Vector2f{ obj->X , obj->Y - obj->Height};
	mOffset = pos - go->getPosition();
}

SpriteRenderer* hfm::SpriteRenderer::clone()
{
	return new SpriteRenderer(*this);
}

sf::Sprite& hfm::SpriteRenderer::getSprite()
{
	return mSprite;
}
