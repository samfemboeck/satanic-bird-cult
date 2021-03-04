#pragma once

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <AnimationRenderer.h>
#include <Collider.h>

using namespace std;

namespace hfm
{
	namespace tld
	{
		struct Animation
		{
			std::string TilesetName;
			std::vector<int> Indices;
			sf::Time FrameTime;
			bool IsLoopable;
			float FrameWidth;
			float FrameHeight;
		};

		struct Tileset
		{
			int FirstGid;
			string Name;
			float TileWidth;
			float TileHeight;
			string Filename;
			std::map<int, Animation> Animations;

			~Tileset()
			{
			}
		};

		struct ObjectProperty
		{
			string Name;
			string Value;
		};

		struct Layer
		{
			string Name;
			int Width;
			int Height;
			int* Data;
			vector<ObjectProperty*> Properties;

			~Layer()
			{
				delete[] Data;
			}
		};

		struct Object
		{
			string Name;
			string Type;
			int Gid;
			float X;
			float Y;
			float Width;
			float Height;
			map<string, string> Properties;
			std::vector<sf::Vector2f> Polygon;
		};


		struct ObjectGroup
		{
			string Name;
			float Width;
			float Height;
			bool IsVisible;
			sf::Color* Color = nullptr;

			vector<Object*> objects;

			~ObjectGroup()
			{
				for (auto o : objects) {
					delete o;
				}

				delete Color;
			}
		};


		struct Map
		{
			float Scale;
			float Width;
			float Height;
			float TileWidth;
			float TileHeight;

			std::vector<Tileset*> Tilesets;
			vector<Layer*> Layers;
			vector<ObjectGroup*> Groups;

			~Map()
			{
				for (auto g : Groups)
				{
					delete g;
				}

				for (auto l : Layers)
				{
					delete l;
				}

				for (auto ts : Tilesets)
				{
					delete ts;
				}
			}

			Tileset* getTilesetForGid(int grid);
		};


		Map* loadMap(char* xml);
	}
}
