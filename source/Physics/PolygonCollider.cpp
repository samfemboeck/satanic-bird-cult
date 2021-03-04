#include "PolygonCollider.h"
#include "GameObject.h"
#include <iostream>
#include "QuickMaths.h"
#include "CircleCollider.h"
#include "PhysicsManager.h"
#include "RigidBody.h"
#include <AABBCollider.h>
// (c) Coding: Femböck Samuel, Hofer Thomas
// FH - Salzburg

using namespace hfm;

PolygonCollider::PolygonCollider(const PolygonCollider& other) : Collider(other)
{
	mShape = other.mShape;
}

PolygonCollider::PolygonCollider(tld::Object* obj, tld::ObjectGroup* group, GameObject* go) : Collider(obj, group, go)
{
    mShape.setPointCount(obj->Polygon.size());

    for (int i = 0; i < obj->Polygon.size(); ++i)
    {
        mShape.setPoint(i, obj->Polygon[i] * Map::Scale);
    }

	mShape.setPosition({ (float)obj->X * Map::Scale, (float)obj->Y * Map::Scale});

	if (group->Color)
		mShape.setOutlineColor(*group->Color);
	else
		mShape.setOutlineColor(sf::Color::Magenta);

	mShape.setOutlineThickness(2);
	mShape.setFillColor(sf::Color::Transparent);
	setOffset(sf::Vector2f(mShape.getPosition().x - go->getPosition().x, mShape.getPosition().y - go->getPosition().y));	
}

PolygonCollider* PolygonCollider::clone()
{
	return new PolygonCollider(*this);
}

void PolygonCollider::update(sf::Time delta)
{
    mShape.setPosition(mGo->getPosition() + getOffset());
}

void PolygonCollider::onStart()
{
	Collider::onStart();
	setOffset(sf::Vector2f(mShape.getPosition().x - mGo->getPosition().x, mShape.getPosition().y - mGo->getPosition().y));
}

struct Projection
{
	float min, max;
};

std::vector<sf::Vector2f> PolygonCollider::getAxes()
{
	std::vector<sf::Vector2f> result;
	auto points = getVertices();
	sf::Vector2f edge;

	for (int i = 0; i < points.size(); ++i)
	{
		edge.x = points[(i + 1) % points.size()].x - points[i].x;
		edge.y = points[(i + 1) % points.size()].y - points[i].y;
		result.push_back(QuickMaths::normalize(QuickMaths::perp(edge)));
	}

	return result;
}

Projection getProjection(const std::vector<sf::Vector2f>& points, const sf::Vector2f& axis)
{
	Projection result;
	result.min = QuickMaths::dot(points[0], axis);
	result.max = result.min;
	float projection;

	for (int i = 1; i < points.size(); ++i)
	{
		projection = QuickMaths::dot(points[i], axis);

		if (projection < result.min)
			result.min = projection;
		else if (projection > result.max)
			result.max = projection;
	}

	return result;
}

std::vector<sf::Vector2f> PolygonCollider::getVertices() 
{
	std::vector<sf::Vector2f> ret(mShape.getPointCount());

	for (int i = 0; i < mShape.getPointCount(); ++i)
	{
		ret[i] = mShape.getPosition() + mShape.getPoint(i);
	}

	return ret;
}

bool PolygonCollider::vsPoly(const std::vector<sf::Vector2f>& verticesA, const std::vector<sf::Vector2f>& axesA, const sf::Vector2f& centerA, 
	const std::vector<sf::Vector2f>& verticesB, const std::vector<sf::Vector2f>& axesB, const sf::Vector2f& centerB, 
	MTV& mtv)
{
	mtv.penetration = std::numeric_limits<float>().max();

	for (int i = 0; i < axesA.size(); ++i)
	{
		Projection proj1 = getProjection(verticesA, axesA[i]);
		Projection proj2 = getProjection(verticesB, axesA[i]);

		double overlap = std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
		if (overlap <= 0)
			return false;

		if (overlap < mtv.penetration)
		{
			mtv.penetration = (float)overlap;
			mtv.normal = axesA[i];
		}
	}

	for (int i = 0; i < axesB.size(); ++i)
	{
		Projection proj1 = getProjection(verticesA, axesB[i]);
		Projection proj2 = getProjection(verticesB, axesB[i]);

		double overlap = std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
		if (overlap <= 0)
			return false;

		if (overlap < mtv.penetration)
		{
			mtv.penetration = (float)overlap;
			mtv.normal = axesB[i];
		}
	}

	if (QuickMaths::dot(centerA - centerB, mtv.normal) >= 0)
				mtv.normal = QuickMaths::negate(mtv.normal);

	return true;
}

bool PolygonCollider::vsCircle(PolygonCollider& poly, CircleCollider& circle, MTV& mtv)
{
	auto p1Points = poly.getVertices();
	std::vector<sf::Vector2f> axes = poly.getAxes();

	auto circleCenter = circle.getGameObject()->getPosition();
	auto p2Points = std::vector<sf::Vector2f>{ circleCenter };

	mtv.penetration = std::numeric_limits<float>().max();

	sf::Vector2f closestPoint;
	float closestDistance = std::numeric_limits<float>().max();
	for (const auto& point : p1Points)
	{
		float lengthSquared = pow(point.x - circleCenter.x, 2) + pow(point.y - circleCenter.y, 2);
		if (lengthSquared < closestDistance)
		{
			closestDistance = lengthSquared;
			closestPoint = point;
		}
	}

	sf::Vector2f circleAxis = QuickMaths::normalize(closestPoint - circleCenter);
	axes.push_back(circleAxis);

	sf::CircleShape& circleShape = dynamic_cast<sf::CircleShape&>(circle.getShape());

	for (int i = 0; i < axes.size(); ++i)
	{
		Projection proj1 = getProjection(p1Points, axes[i]);
		Projection proj2 = getProjection(p2Points, axes[i]);
		proj2.min -= circleShape.getRadius();
		proj2.max += circleShape.getRadius();

		double overlap = std::max(0.f, std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min));
		if (overlap <= 0)
			return false;

		if (overlap < mtv.penetration)
		{
			mtv.penetration = (float)overlap;
			mtv.normal = axes[i];
		}
	}

	if (QuickMaths::dot(poly.getCenter() - circleCenter, mtv.normal) >= 0)
		mtv.normal = QuickMaths::negate(mtv.normal);

	return true;
}

sf::Shape& PolygonCollider::getShape()
{
	return mShape;
}

sf::Vector2f PolygonCollider::getCenter()
{
	float X = 0, Y = 0;

	for (const auto& point : getVertices())
	{
		X += point.x;
		Y += point.y;
	}

	return sf::Vector2f(X / mShape.getPointCount(), Y / mShape.getPointCount());
}

bool PolygonCollider::collidesWith(Collider* collider, MTV& mtv)
{
	PolygonCollider* poly = dynamic_cast<PolygonCollider*>(collider);

	if (poly)
	{
		return PolygonCollider::vsPoly(getVertices(), getAxes(), getCenter(), poly->getVertices(), poly->getAxes(), poly->getCenter(), mtv);
	}

	CircleCollider* circle = dynamic_cast<CircleCollider*>(collider);

	if (circle)
	{
		return PolygonCollider::vsCircle(*this, *circle, mtv);
	}

	AABBCollider* box = dynamic_cast<AABBCollider*>(collider);

	if (box)
	{
		return PolygonCollider::vsPoly(getVertices(), getAxes(), getCenter(), box->getVertices(), { {1, 0}, {0, 1}, {-1, 0}, {0, -1} }, box->getCenter(), mtv);
	}
	
    return false;
}
