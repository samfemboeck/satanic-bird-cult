#include <cassert>
#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include "TiledMap.h"
#include <SFML/Graphics/Color.hpp>
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "AABBCollider.h"
#include "Map.h"
#include "Animation.h"
#pragma warning(disable: 4996)
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace rapidxml;
using namespace hfm;

tld::Tileset* tld::Map::getTilesetForGid(int grid)
{
	tld::Tileset* result = nullptr;

	for (auto ts : Tilesets)
	{
		if (ts->FirstGid > grid)
		{
			break;
		}

		result = ts;
	}

	return result;
}

tld::Map* tld::loadMap( char *xml )
{
    xml_document<> doc;
	doc.parse<0>(xml);
    
    xml_node<> *mapnode = doc.first_node("map");
    
    tld::Map* map = new tld::Map();
    
    map->Width = atof( mapnode->first_attribute( "width" )->Value() );
    map->Height = atof( mapnode->first_attribute( "height" )->Value() );
    map->TileWidth = atof( mapnode->first_attribute( "tilewidth" )->Value() );
    map->TileHeight = atof( mapnode->first_attribute( "tileheight" )->Value() );
    
    xml_node<> *tilesetnode = mapnode->first_node( "tileset" );
    
    while ( tilesetnode ) {
        tld::Tileset* tileset = new tld::Tileset();
        
        tileset->FirstGid = atof( tilesetnode->first_attribute( "firstgid" )->Value() );
        tileset->Name = tilesetnode->first_attribute( "name" )->Value();
        tileset->TileWidth =  atof( tilesetnode->first_attribute( "tilewidth" )->Value() );
        tileset->TileHeight = atof( tilesetnode->first_attribute( "tileheight" )->Value() );
        tileset->Filename = tilesetnode->first_node( "image" )->first_attribute( "source" )->Value();

        xml_node<>* tilenode = tilesetnode->first_node("tile");
        while (tilenode)
        {
			auto animationnode = tilenode->first_node("animation");
            if (animationnode)
            {
				Animation anim;
				anim.FrameTime = sf::milliseconds(std::stof(animationnode->first_node("frame")->first_attribute("duration")->Value()));
				anim.IsLoopable = true;
				int idx = std::stoi(tilenode->first_attribute("id")->Value());
				anim.FrameWidth = tileset->TileWidth;
				anim.FrameHeight = tileset->TileHeight;

                while (animationnode)
                {
                    auto framenode = animationnode->first_node("frame");
                    while (framenode)
                    {
                        int idx = std::stoi(framenode->first_attribute("tileid")->Value());
                        anim.Indices.push_back(idx);
                        framenode = framenode->next_sibling("frame");
                    }

					tileset->Animations[idx] = anim;
					animationnode = animationnode->next_sibling("animation");
				}
			}

			tilenode = tilenode->next_sibling("tile");
		}

		map->Tilesets.push_back(tileset);

		tilesetnode = tilesetnode->next_sibling("tileset");
	}

	const char* separators = " \t,\n\r";

	xml_node<>* layernode = mapnode->first_node("layer");

	while (layernode) {
		tld::Layer* layer = new tld::Layer();

		layer->Name = layernode->first_attribute("name")->Value();
		layer->Width = atoi(layernode->first_attribute("width")->Value());
		layer->Height = atoi(layernode->first_attribute("height")->Value());

		xml_node<>* propertiesNode = layernode->first_node("properties");

		if (propertiesNode) {
			xml_node<>* propertyNode = propertiesNode->first_node("property");
			while (propertyNode) {
				tld::ObjectProperty* property = new tld::ObjectProperty();
				property->Name = propertyNode->first_attribute("name")->Value();
				property->Value = propertyNode->first_attribute("value")->Value();

				layer->Properties.push_back(property);

				propertyNode = propertyNode->next_sibling("property");
			}
		}

		const char* Data = layernode->first_node("data")->Value();

		assert(strcmp(Data, "csv"));

		layer->Data = new int[layer->Width * layer->Height];

		char* copy = (char*)malloc(strlen(Data) + 1);
		strcpy(copy, Data);
		char* item = strtok(copy, separators);

		int index = 0;
		while (item) {
			layer->Data[index] = atoi(item);
			index++;

			item = strtok(0, separators);
		}

		free(copy);

		map->Layers.push_back(layer);

		layernode = layernode->next_sibling("layer");
	}

	xml_node<>* objectgroupnode = mapnode->first_node("objectgroup");

	while (objectgroupnode) {
		tld::ObjectGroup* group = new tld::ObjectGroup();

		group->Name = objectgroupnode->first_attribute("name")->Value();
		auto Color = objectgroupnode->first_attribute("color");
		if (Color)
		{
			int r, g, b;
			sscanf(Color->Value() + 1, "%02x%02x%02x", &r, &g, &b);
			group->Color = new sf::Color(r, g, b);
		}

		//group->Width = atoi( objectgroupnode->first_attribute( "Width" )->Value() );
		//group->Height = atoi( objectgroupnode->first_attribute( "Height" )->Value() );

		xml_attribute<>* visibleattr = objectgroupnode->first_attribute("visible");
		if (visibleattr) {
			group->IsVisible = (atoi(visibleattr->Value()) != 0);
		}
		else {
			group->IsVisible = true;
		}

		//cout << "group " << group->Name << endl;

		xml_node<>* objectnode = objectgroupnode->first_node("object");

		while (objectnode) {
			tld::Object* object = new tld::Object();

			auto nameattr = objectnode->first_attribute("name");
			if (nameattr) {
				object->Name = nameattr->Value();
			}
			auto gidattr = objectnode->first_attribute("gid");
			if (gidattr) {
				object->Gid = atoi(gidattr->Value());
			}
			auto typeattr = objectnode->first_attribute("type");
			if (typeattr) {
				object->Type = typeattr->Value();
			}
			object->X = atof(objectnode->first_attribute("x")->Value());
			object->Y = atof(objectnode->first_attribute("y")->Value());

			auto widthattr = objectnode->first_attribute("width");
			if (widthattr) {
				object->Width = atof(widthattr->Value());
			}

			auto heightattr = objectnode->first_attribute("height");
			if (heightattr) {
				object->Height = atof(heightattr->Value());
			}

			xml_node<>* propertiesnode = objectnode->first_node("properties");

			if (propertiesnode) {

				xml_node<>* propertynode = propertiesnode->first_node("property");

				while (propertynode) {
					tld::ObjectProperty* property = new tld::ObjectProperty();
					property->Name = propertynode->first_attribute("name")->Value();
					property->Value = propertynode->first_attribute("value")->Value();
					object->Properties[property->Name] = property->Value;
					propertynode = propertynode->next_sibling("property");
				}
			}

			xml_node<>* polygonnode = objectnode->first_node("polygon");

			if (polygonnode)
			{
				std::string pointsRaw = polygonnode->first_attribute("points")->Value();

				size_t pos = 0;
				std::vector<std::string> points;
				while ((pos = pointsRaw.find(" ")) != std::string::npos) {
					points.push_back(pointsRaw.substr(0, pos));
					pointsRaw.erase(0, pos + 1);
				}
				points.push_back(pointsRaw);

				std::vector<sf::Vector2f> points2f;
				for (std::string& point : points)
				{
					sf::Vector2f ret;
					pos = point.find(",");
					ret.x = std::stof(point.substr(0, pos));
					ret.y = std::stof(point.substr(pos + 1, point.size()));
					points2f.push_back(ret);
				}

				object->Polygon = points2f;
			}

			group->objects.push_back(object);

			objectnode = objectnode->next_sibling("object");
		}

		map->Groups.push_back(group);

		objectgroupnode = objectgroupnode->next_sibling("objectgroup");
	}

	//free( (void*) xml );

	return map;
}
