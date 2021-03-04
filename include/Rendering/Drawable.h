#pragma once
#include "Component.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}

	class Drawable : public Component
	{
	public:
		Drawable() = default;
		Drawable(const Drawable& other);
		Drawable(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

		virtual void draw(sf::RenderWindow& window) = 0;

		void onDisable() override;
		void onEnable() override;

		virtual Drawable* clone() = 0;
		void setVisible(bool isVisible);
		bool isVisible();
		std::string getLayer();
		void setLayer(std::string layer);
		void setOffset(sf::Vector2f offset);
		sf::Vector2f getOffset();

	protected:
		sf::Vector2f mOffset;

	private:
		bool mIsVisible = true;
		std::string mLayer = "default";
	};
};