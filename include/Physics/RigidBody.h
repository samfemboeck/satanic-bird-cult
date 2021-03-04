#pragma once
#include "Component.h"
#include <vector>
#include <list>
#include "TiledMap.h"

namespace hfm
{
	class Collider;
	class IRigidBodyObserver;

	class RigidBody : public Component
	{
	public:
		RigidBody(const RigidBody& other);
		RigidBody(tld::Object* obj, tld::ObjectGroup* group, GameObject* go);
		RigidBody() = delete;
		~RigidBody() override;

		RigidBody* clone() override;
		void onAwake() override;
		void onStart() override;
		void onEnable() override;
		void onDisable() override;

		void correctPosition(sf::Time delta);
		void onCollision(Collider* other);
		float getInvMass();
		void setInvMass(float Value);
		sf::Vector2f getAcceleration();
		void setAcceleration(sf::Vector2f acc);
		sf::Vector2f getVelocity();
		void setVelocity(sf::Vector2f vel);
		void addForce(sf::Vector2f force);
		void addImpulse(sf::Vector2f impulse);
		std::list<sf::Vector2f>& getForces();
		std::list<sf::Vector2f>& getImpulses();
		void setIgnore(std::string layer, bool ignore);
		void setIgnore(RigidBody* rb, bool ignore);
		bool hasIgnore(RigidBody* rb);
		std::string getLayer();
		void setLayer(std::string layer);
		void addCorrection(const sf::Vector2f& correction);

	private:
		sf::Vector2f mCorrection;
		float mInvMass = 1;
		sf::Vector2f mAcceleration;
		sf::Vector2f mVelocity;
		std::list<sf::Vector2f> mForces;
		std::list<sf::Vector2f> mImpulses;
		std::vector<IRigidBodyObserver*> mObservers;
		std::string mLayer = "default";
		std::map<std::string, bool> mIgnoredLayers;
		std::map<RigidBody*, bool> mIgnoredRbs;
	};
}