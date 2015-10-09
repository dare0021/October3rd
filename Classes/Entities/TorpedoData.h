#ifndef __TORPEDODATA_H__
#define __TORPEDODATA_H__

#include "cocos2d.h"
#include "AnimData.h"

class TorpedoData
{
public:
	TorpedoData(std::string name);

	std::string getName();

	std::string spritePath;
    PhysicsModel physicsModel;
    ///See O3Sprite.friction
    float friction;
    float initialSpeed, mass, initialForce, maxSpeed, maxForce, turnSpeed, headingOffset;
    cocos2d::Vec2 positionOffset;

    bool animated;
    std::unordered_map<std::string, AnimData*> animations;
    std::string initialAnimation;

private:
	std::string name;
};

#endif // __TORPEDODATA_H__
