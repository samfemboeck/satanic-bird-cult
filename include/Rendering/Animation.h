#pragma once
#include "Component.h"
#include "SFML/System/Time.hpp"

namespace sf
{
	class Texture;
}

namespace hfm
{
	namespace tld
	{
		struct Object;
		struct ObjectGroup;
	}

	class Animation : public Component
	{
	public:
		Animation() = delete;
		Animation(const Animation& other);
		Animation(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);

		Animation* clone() override;
		void onAwake() override;

		std::string getName();
		sf::IntRect& getActiveFrame();
		bool goToNextIndex();
		sf::Texture* getTexture();
		sf::Time getFrameTime();
		bool isLoopable();
		std::vector<int>& getIndices();
		void addFrame(sf::IntRect& frame);
		int getFrameWidth();
		int getFrameHeight();
		sf::Vector2f getOffset();
		sf::Vector2f getScale();
		void setTexture(sf::Texture* tex);

	private:
		std::string mName;
		sf::Time mFrameTime;
		int mFrameidx;
		bool mIsLoopable;
		std::vector<sf::IntRect> mFrames;
		std::vector<int> mIndices;
		sf::Texture* mTilesetTexture;
		int mFrameWidth;
		int mFrameHeight;
		sf::Vector2f mOffset;
		sf::Vector2f mOrigin;
		sf::Vector2f mScale;
	};
}

