#include "ContentManager.h"
#include <SFML/System/FileInputStream.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "LayerRenderer.h"
#include "GameObjectManager.h"
#include <functional>
#include "RigidBody.h"
#include "CircleCollider.h"
#include "AABBCollider.h"
#include "PolygonCollider.h"
#include "AnimationRenderer.h"
#include "Animation.h"
#include "Map.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;
using namespace hfm::tld;

ContentManager::ContentManager()
{
	mCreators["SpriteRenderer"] = creator<SpriteRenderer>;
	mCreators["AnimationRenderer"] = creator<AnimationRenderer>;
	mCreators["Animation"] = creator<hfm::Animation>;
	mCreators["RigidBody"] = creator<RigidBody>;
	mCreators["PolygonCollider"] = creator<PolygonCollider>;
	mCreators["AABBCollider"] = creator<AABBCollider>;
	mCreators["CircleCollider"] = creator<CircleCollider>;
	mCreators["Map"] = creator<Map>;
}

void ContentManager::loadMap(const string& Filename, const sf::Vector2f& offset)
{
	sf::FileInputStream mapStream;
	if (!mapStream.open(Filename))
	{
		sf::err() << "loadMap: could not open file " << Filename << endl;
		return;
	}

	// convert FileInputStream to char* mapBuffer
	char* mapBuffer = new char[mapStream.getSize() + 1];
	mapStream.read(mapBuffer, mapStream.getSize());
	mapBuffer[mapStream.getSize()] = '\0';

	mMap = std::unique_ptr<tld::Map>(tld::loadMap(mapBuffer));
	delete[] mapBuffer;

	sf::err() << "Load mMap with size: " << mMap->Width << ", "
		<< mMap->Height << " and tilesize: " << mMap->TileWidth
		<< ", " << mMap->TileHeight << std::endl;

	// load textures for every tileset
	for (const auto tileset : mMap->Tilesets)
	{
		sf::err() << "Load tileset: " << tileset->Name << " width filename: "
			<< tileset->Filename << " and tilesize: " << tileset->TileWidth
			<< ", " << tileset->TileHeight << std::endl;

		auto texture = make_shared<sf::Texture>();
		if (!texture->loadFromFile(mAssetsPath + tileset->Filename))
			sf::err() << "Could not load texture " << mAssetsPath + tileset->Filename << endl;

		mTilesetTextures[tileset->Name] = texture;
	}

	std::map<std::string, GameObject*> tmpObjects;

	for (int layerIdx = 0; layerIdx < (int)mMap->Layers.size(); layerIdx++)
	{
		tld::Layer* layer = mMap->Layers[layerIdx];

		sf::err() << "Load layer: " << layer->Name << " with width: "
			<< layer->Width << " and height: " << layer->Height << std::endl;

		int size = layer->Width * layer->Height;
		auto go = new GameObject(layer->Name);
		auto& layerRenderer = go->addComponent<LayerRenderer>();
		layerRenderer.setLayer(layer->Name);
		

		// iterate over all elements/Tiles in the layer
		for (int i = 0; i < size; i++)
		{
			int Gid = layer->Data[i];

			if (Gid == 0)
			{
				// 0 means there is no tile at this position.
				continue;
			}

			sf::Sprite sprite = getSpriteForGid(Gid);

			// calculate position of tile
			sf::Vector2f position;
			sf::Vector2i tileSize(mMap->TileWidth, mMap->TileHeight);
			position.x = (i % layer->Width) * (float)tileSize.x;
			position.y = (i / layer->Width) * (float)tileSize.y;
			position += offset;

			sprite.setPosition(position.x * hfm::Map::Scale, position.y * hfm::Map::Scale);

			layerRenderer.add(std::unique_ptr<sf::Sprite>(new sf::Sprite(sprite)));
		}

		tmpObjects[go->getName()] = go;
	}

	for (auto group : mMap->Groups)
	{
		for (auto object : group->objects)
		{
			if (object->Type == "GameObject")
			{
				tmpObjects[object->Name] = new GameObject(object->Name, object);
			}
		}
	}

	for (auto group : mMap->Groups)
	{
		auto goIt = tmpObjects.find(group->Name);
		if (goIt == tmpObjects.end())
			continue;

		for (auto object : group->objects)
		{
			if (object->Type == "GameObject")
				continue;

			auto creator = mCreators[object->Type];
			if (!creator)
				throw "Creator not found for Component. Component type was: " + object->Type;

			creator(object, group, goIt->second);
		}
	}

	for (const auto& pair : tmpObjects)
	{
		auto go = pair.second;
		if (go->isTemplate())
			GameObjectManager::getInstance().addTemplate(go);
		else
			GameObjectManager::getInstance().add(go);
	}

	GameObjectManager::getInstance().update(sf::seconds(0));
}

void hfm::ContentManager::addCreator(std::string name, Creator creator)
{
	mCreators[name] = creator;
}

ContentManager& ContentManager::getInstance()
{
	static ContentManager instance;
	return instance;
}

void ContentManager::clear()
{
	mTilesetTextures.clear();
	mCreators.clear();
}

tld::Map& ContentManager::getMap()
{
	return *mMap;
}

sf::Texture* ContentManager::getTexture(std::string name)
{
	return mTilesetTextures[name].get();
}

sf::Sprite hfm::ContentManager::getSpriteForGid(int gid)
{
	// get tileset for tile and allocate the corresponding tileset texture
	tld::Tileset* tileset = mMap->getTilesetForGid(gid);
	sf::Vector2i tileSize(tileset->TileWidth, tileset->TileHeight);
	sf::Texture& texture = *mTilesetTextures[tileset->Name];

	// number of Tiles in tileset texture (horizontally)
	int tileCountX = texture.getSize().x / tileSize.x;

	// calculate 2d idx of tile in tileset texture
	int idx = gid - tileset->FirstGid;
	int idxX = idx % tileCountX;
	int idxY = idx / tileCountX;

	// calculate source area of tile in tileset texture
	sf::IntRect source(idxX * tileSize.x, idxY * tileSize.y, tileSize.x, tileSize.y);

	// create tile (Sprite) and put it into the appropriate layer.
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(source);
	sprite.setScale(hfm::Map::Scale, hfm::Map::Scale);

	return sprite;
}
