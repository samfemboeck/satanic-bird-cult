#pragma once
#include <unordered_map>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <memory>
#include <map>
#include "SpriteRenderer.h"
#include "TiledMap.h"
#include "GameObject.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	typedef std::shared_ptr<sf::Texture> TexturePtr;
	typedef std::shared_ptr<SpriteRenderer> SpriteRendererPtr;
	typedef void(*Creator)(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

	template<typename T>
	void creator(tld::Object* obj, tld::ObjectGroup* group, GameObject* go)
	{
		auto comp = std::make_unique<T>(obj, group, go);
		go->addComponent(std::move(comp));
	}

	class ContentManager
	{
	public:
		void loadMap(const std::string& Filename, const sf::Vector2f& offset);
		void addCreator(std::string name, Creator creator);
		static ContentManager& getInstance();
		void clear();
		tld::Map& getMap();
		sf::Texture* getTexture(std::string name);
		sf::Sprite getSpriteForGid(int gid);

	private:
		ContentManager();
		const std::string mAssetsPath = "../assets/";
		std::unordered_map <std::string, TexturePtr> mTilesetTextures;
		std::unordered_map <std::string, Creator> mCreators;
		std::unique_ptr<tld::Map> mMap;
	};
}
