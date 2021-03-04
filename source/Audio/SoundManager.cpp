#include "SoundManager.h"
#include <iostream>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

SoundManager& SoundManager::getInstance()
{
	static SoundManager instance;
	return instance;
}

void SoundManager::play(std::string Name)
{
	if (mSounds.size() >= 256)
		return;

	std::unique_ptr<sf::Sound> sound = std::make_unique<sf::Sound>();
	sound->setBuffer(*mSoundBuffers[Name]);
	sound->play();
	mSounds.push_back(std::move(sound));
}

void SoundManager::addSound(std::string Filename, std::string Name)
{
	std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
	buffer->loadFromFile(Filename);
	mSoundBuffers.insert(std::make_pair(Name,std::move(buffer)));
}

void SoundManager::playMusic(std::string Filename)
{
	if (!mMusic.openFromFile(Filename))
	{
		throw "Music can't be loaded.";
	}

	mMusic.setVolume(50.0f);
	mMusic.play();
	mMusic.setLoop(true);
}

void SoundManager::pauseMusic()
{
	mMusic.pause();
}

void SoundManager::resumeMusic()
{
	mMusic.play();
}

void SoundManager::update()
{
	if (mSounds.size() > 0)
	{
		if (mSounds.front()->getStatus() == sf::Sound::Status::Stopped)
		{
			mSounds.pop_front();
		}
	}
}

SoundManager::SoundManager()
{
}
