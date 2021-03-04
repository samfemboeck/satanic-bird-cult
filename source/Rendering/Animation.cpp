#include "Animation.h"
#include "TiledMap.h"
#include "ContentManager.h"
#include "GameObject.h"
#include "Debug.h"

using namespace hfm;

Animation::Animation(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Component(obj, group, go)
{
    tld::Tileset* tileset = ContentManager::getInstance().getMap().getTilesetForGid(obj->Gid);
    int index = obj->Gid - tileset->FirstGid;
    tld::Animation anim = tileset->Animations[index];
    mIndices = anim.Indices;
    mFrameTime = anim.FrameTime;
    mTilesetTexture = ContentManager::getInstance().getTexture(tileset->Name);
    mIsLoopable = anim.IsLoopable;
    mFrameWidth = anim.FrameWidth;
    mFrameHeight = anim.FrameHeight;
    mScale = sf::Vector2f{ obj->Width / mFrameWidth, obj->Height / mFrameHeight };
    mOrigin = Map::Scale * sf::Vector2f{ (obj->X + mFrameWidth * mScale.x * 0.5f), (obj->Y - mFrameHeight * mScale.y * 0.5f)};
    mName = obj->Name;

	sf::Vector2u size = mTilesetTexture->getSize();
	int columnFrames = size.x / mFrameWidth;

	for (const int i : mIndices)
	{
		sf::Rect<int> rect((mFrameWidth * i) % size.x, mFrameHeight * (int)(i / columnFrames), mFrameWidth, mFrameHeight);
		addFrame(rect);
	}

	mOffset = go->getPosition() - mOrigin;
    
    if (obj->Properties.find("isLoopable") != obj->Properties.end())
	{
		std::istringstream(obj->Properties["isLoopable"]) >> std::boolalpha >> mIsLoopable;
	}
}

Animation::Animation(const Animation& other)
{
    mIndices = other.mIndices;
    mFrameTime = other.mFrameTime;
    mTilesetTexture = other.mTilesetTexture;
    mIsLoopable = other.mIsLoopable;
    mFrameWidth = other.mFrameWidth;
    mFrameHeight = other.mFrameHeight;
    mScale = other.mScale;
    mOffset = other.mOffset;
    mName = other.mName;
    mFrames = other.mFrames;
}

Animation* hfm::Animation::clone()
{
    return new Animation(*this);
}

void hfm::Animation::onAwake()
{
}

std::string Animation::getName()
{
    return mName;
}

sf::IntRect& Animation::getActiveFrame()
{
    return mFrames[mFrameidx];
}

bool Animation::goToNextIndex()
{
    if (mFrameidx == mIndices.size() - 1)
    {
        if (mIsLoopable)
			mFrameidx = 0;

        return false;
    }

    ++mFrameidx;
    return true;
}

sf::Texture* hfm::Animation::getTexture()
{
    return mTilesetTexture;
}

sf::Time hfm::Animation::getFrameTime()
{
    return mFrameTime;
}

bool hfm::Animation::isLoopable()
{
    return mIsLoopable;
}

std::vector<int>& Animation::getIndices()
{
    return mIndices;
}

void hfm::Animation::addFrame(sf::IntRect& frame)
{
    mFrames.push_back(frame);
}

int hfm::Animation::getFrameWidth()
{
    return mFrameWidth;
}

int hfm::Animation::getFrameHeight()
{
    return mFrameHeight;
}

sf::Vector2f hfm::Animation::getOffset()
{
    return mOffset;
}

sf::Vector2f hfm::Animation::getScale()
{
    return mScale * Map::Scale;
}

void hfm::Animation::setTexture(sf::Texture* tex)
{
    mTilesetTexture = tex;
}

