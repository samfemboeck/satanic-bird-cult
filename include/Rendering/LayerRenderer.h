#pragma once
#include "Drawable.h"
#include <SFML\Graphics\Sprite.hpp>
#include <vector>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class LayerRenderer : public Drawable
	{
	public:
		LayerRenderer() = default;
		LayerRenderer(const LayerRenderer& other);

		virtual void onEnable() override;
		virtual LayerRenderer* clone() override;
		virtual void draw(sf::RenderWindow& window) override;

		void add(std::unique_ptr<sf::Sprite> sprite);
		void setScale(float scale);
		float getScale();

	private:
		float mScale = 1;
		std::vector<std::unique_ptr<sf::Sprite>> mSprites;
		sf::Transformable mTransformable;
	};
}

