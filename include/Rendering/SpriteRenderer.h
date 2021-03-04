#pragma once
#include "Drawable.h"
#include <string>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}

	class SpriteRenderer : public Drawable
	{
	public:
		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer& other);
		SpriteRenderer(hfm::tld::Object* obj, hfm::tld::ObjectGroup* group, GameObject* go);

		virtual SpriteRenderer* clone() override;
		virtual void onStart();
		virtual void onAwake() override;
		virtual void draw(sf::RenderWindow&) override;

		sf::Sprite& getSprite();

	protected:
		sf::Sprite mSprite;
	};
}