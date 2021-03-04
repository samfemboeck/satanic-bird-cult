#pragma once
#include "Drawable.h"
#include <map>

namespace hfm
{
	class RenderManager
	{
	public:
		static RenderManager& getInstance();
		void add(Drawable* renderComponent);
		void add(Drawable* drawable, int layer);
		void draw(sf::RenderWindow& window);
		void remove(Drawable* renderComponent);
		void addLayer(std::string layer, int idx);
		RenderManager();

	private:
		std::vector<std::vector<Drawable*>> mRenderList;
		std::map<std::string, int> mLayerMap;
		const int MAX_LAYERS = 100;
	};
}