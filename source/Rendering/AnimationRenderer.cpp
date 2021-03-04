#include "AnimationRenderer.h"
#include <iostream>
#include "GameObjectManager.h"
#include "TiledMap.h"
#include "ContentManager.h"
#include "Animation.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

AnimationRenderer::AnimationRenderer(const AnimationRenderer& other) : Drawable(other)
{
    mActiveAnimation = other.mActiveAnimation;
}

AnimationRenderer::AnimationRenderer(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Drawable(obj, group, go)
{
    if (obj->Properties.find("activeAnimation") != obj->Properties.end())
    {
        mActiveAnimation = obj->Properties["activeAnimation"];
    }
}

AnimationRenderer* hfm::AnimationRenderer::clone()
{
    return new AnimationRenderer(*this);
}

void AnimationRenderer::onAwake()
{
    auto anims = mGo->getComponents<Animation>();
    for (const auto anim : anims)
    {
        addAnimation(anim);
    }

    play(mActiveAnimation);
}

void AnimationRenderer::update(sf::Time delta)
{
    if (!mIsPaused)
    {
        mElapsedTime += delta;
        auto activeAnim = mAnimations[mActiveAnimation];

        if (mElapsedTime.asSeconds() >= activeAnim->getFrameTime().asSeconds())
        {
            mElapsedTime = sf::microseconds(mElapsedTime.asMicroseconds() % activeAnim->getFrameTime().asMicroseconds());

            if (!activeAnim->goToNextIndex() && !activeAnim->isLoopable())
            {
				mIsPaused = true;
            }

            mSprite.setTextureRect(activeAnim->getActiveFrame());
        }
    }
}

void hfm::AnimationRenderer::draw(sf::RenderWindow& window)
{
    auto transform = mGo->getTransform();
    transform.translate(-mOffset);
    window.draw(mSprite, transform);
}

void AnimationRenderer::addAnimation(Animation* animation)
{
	mAnimations[animation->getName()] = animation;
}

void AnimationRenderer::play(std::string Name)
{
    mActiveAnimation = Name;
    auto anim = mAnimations[Name];
    mFrameWidth = anim->getFrameWidth();
    mFrameHeight = anim->getFrameHeight();
    mSprite.setTexture(*mAnimations[mActiveAnimation]->getTexture());
    mSprite.setTextureRect(anim->getActiveFrame());
    mSprite.setOrigin(mFrameWidth * 0.5f, mFrameHeight * 0.5f);
    mSprite.setScale(anim->getScale());
    mOffset = anim->getOffset();
    mIsPaused = false;
}

void AnimationRenderer::setActive(bool isActive)
{
    mIsPaused = !isActive;
}

std::string AnimationRenderer::getActiveAnimation()
{
    return mAnimations[mActiveAnimation]->getName();
}

sf::Sprite& hfm::AnimationRenderer::getSprite()
{
    return mSprite;
}
