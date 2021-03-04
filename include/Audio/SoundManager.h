#pragma once
#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include <list>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

namespace hfm
{
	class SoundManager
	{
	public:
		static SoundManager& getInstance();
		void play(std::string Name);
		void addSound(std::string Filename, std::string Name);
		void playMusic(std::string Filename);
		void pauseMusic();
		void resumeMusic();

		void update();
		SoundManager();


	private:
		sf::Music mMusic;
		std::list<std::unique_ptr<sf::Sound>> mSounds;
		std::map<std::string, std::unique_ptr<sf::SoundBuffer>> mSoundBuffers;
	};
}