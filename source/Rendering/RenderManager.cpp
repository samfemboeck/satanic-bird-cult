#include "RenderManager.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

void RenderManager::draw(sf::RenderWindow& window)
{
	for (const auto& layer : mRenderList) 
	{
		for (Drawable* drawable : layer) 
		{
			if (drawable->isVisible())
				drawable->draw(window);
		}
	}
}

void RenderManager::add(Drawable* renderComponent)
{
	if (mLayerMap.find(renderComponent->getLayer()) == mLayerMap.end())
		throw "Layer not registered in LayerMap.";

	int idx = mLayerMap[renderComponent->getLayer()];
	mRenderList[idx].push_back(renderComponent);
}

void hfm::RenderManager::add(Drawable* drawable, int layer)
{
	mLayerMap[drawable->getLayer()] = layer;
	mRenderList[layer].push_back(drawable);
}

void RenderManager::remove(Drawable* renderComponent)
{
	int idx = mLayerMap[renderComponent->getLayer()];

	if (std::find(mRenderList[idx].begin(), mRenderList[idx].end(), renderComponent) != mRenderList[idx].end())
		mRenderList[idx].erase(std::find(mRenderList[idx].begin(), mRenderList[idx].end(), renderComponent));
}

void hfm::RenderManager::addLayer(std::string layer, int idx)
{
	mLayerMap[layer] = idx;
}

RenderManager& RenderManager::getInstance()
{
	static RenderManager instance;
	return instance;
}

RenderManager::RenderManager()
{
	mRenderList.resize(MAX_LAYERS);
	mLayerMap["default"] = 0;
	mLayerMap["collider"] = MAX_LAYERS - 1;
}
