#include "LayerRenderer.h"
#include "RenderManager.h"

using namespace hfm;

void LayerRenderer::add(std::unique_ptr<sf::Sprite> sprite)
{
	mSprites.push_back(std::move(sprite));
}

LayerRenderer::LayerRenderer(const LayerRenderer& other)
{
	mScale = other.mScale;
	mTransformable = other.mTransformable;

	for (const auto& ptr : other.mSprites)
	{
		mSprites.push_back(std::unique_ptr<sf::Sprite>(new sf::Sprite(*ptr)));
	}
}

void hfm::LayerRenderer::onEnable()
{
	RenderManager::getInstance().add(this, 0);
}

LayerRenderer* LayerRenderer::clone()
{
	return new LayerRenderer(*this);
}

void LayerRenderer::draw(sf::RenderWindow& window)
{
	for (auto& sprite : mSprites)
		window.draw(*sprite, mTransformable.getTransform());
}

void LayerRenderer::setScale(float scale)
{
	mScale = scale;
}

float LayerRenderer::getScale()
{
	return mScale;
}
