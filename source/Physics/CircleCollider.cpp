#include "CircleCollider.h"
#include "AABBCollider.h"
#include "GameObject.h"
#include "QuickMaths.h"
#include "PhysicsManager.h"
#include "PolygonCollider.h"
#include "Debug.h"
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

CircleCollider* CircleCollider::clone()
{
	return new CircleCollider(*this);
}

bool CircleCollider::collidesWith(Collider* collider, MTV& mtv)
{
	
	AABBCollider* box = dynamic_cast<AABBCollider*>(collider);

	if (box)
	{
		return AABBCollider::vsCircle(dynamic_cast<sf::RectangleShape&>(box->getShape()), mShape, mtv);
	}

	CircleCollider* circle = dynamic_cast<CircleCollider*>(collider);

	if (circle)
	{
		return vsCircle(mShape, dynamic_cast<sf::CircleShape&>(circle->getShape()), mtv);
	}

	PolygonCollider* poly = dynamic_cast<PolygonCollider*>(collider);
	
	if (poly)
	{
		return PolygonCollider::vsCircle(*poly, *this, mtv);
	}
}

void CircleCollider::update(sf::Time)
{
	mShape.setPosition(mGo->getPosition() + getOffset());
}

sf::Shape& CircleCollider::getShape()
{
	return mShape;
}

bool CircleCollider::vsCircle(sf::CircleShape& a, sf::CircleShape& b, MTV& mtv)
{
	sf::Vector2f difference = sf::Vector2f(a.getPosition().x + a.getRadius(),a.getPosition().y + a.getRadius()) 
		- sf::Vector2f(b.getPosition().x + b.getRadius(), b.getPosition().y + b.getRadius());
	float length = QuickMaths::length(difference);
	float radiusSum = a.getRadius() + b.getRadius();

	if (length == 0)
	{
		mtv.normal = sf::Vector2f(-1 , -1 );
		mtv.penetration = -1;
		return true;
	}

	if (length < radiusSum)
	{
		mtv.normal = sf::Vector2f(-1 / length * difference.x, -1 / length * difference.y);
		mtv.penetration = radiusSum - length;
		return true;
	}

	return false;

}

CircleCollider::CircleCollider(const CircleCollider& other) : Collider(other)
{
	mShape = other.mShape;
}

CircleCollider::CircleCollider(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Collider(obj, group, go)
{
	mShape.setRadius(obj->Height * 0.5f * Map::Scale);
	mShape.setPosition({ (float)obj->X * Map::Scale, (float)obj->Y * Map::Scale});

	if (group->Color)
		mShape.setOutlineColor(*group->Color);
	else
		mShape.setOutlineColor(sf::Color::Magenta);

	mShape.setOutlineThickness(2);
	mShape.setFillColor(sf::Color::Transparent);
	setOffset(sf::Vector2f(mShape.getPosition().x - go->getPosition().x, mShape.getPosition().y - go->getPosition().y));	
}
