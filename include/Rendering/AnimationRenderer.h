#pragma once
#include <vector>
#include <map>
#include <string>
#include "SpriteRenderer.h"

// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class Collider;
	class Animation;

	class AnimationRenderer : public Drawable
	{
	public:
		AnimationRenderer(const AnimationRenderer& other);
		AnimationRenderer(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

		virtual AnimationRenderer* clone() override;
		virtual void onAwake() override;
		virtual void update(sf::Time delta) override;
		virtual void draw(sf::RenderWindow& window);

		void addAnimation(Animation* animation);
		void play(std::string mName);
		void setActive(bool isActive);
		std::string getActiveAnimation();
		sf::Sprite& getSprite();

	private:
		sf::Time mElapsedTime;
		bool mIsPaused = true;
		std::string mActiveAnimation;
		std::map<std::string, Animation*> mAnimations;
		int mFrameWidth = 0;
		int mFrameHeight = 0;
		sf::Sprite mSprite;
	};
}
