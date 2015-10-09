#ifndef __TORPEDODATA_H__
#define __TORPEDODATA_H__

#include "cocos2d.h"

class TorpedoData
{
public:
	TorpedoData(std::string path, std::string name);

private:
	std::string name, spritePath;
    PhysicsModel physicsModel;
    ///See O3Sprite.friction
    float friction;
    float initialSpeed, mass, initialForce, maxSpeed, maxForce, turnSpeed, headingOffset;

    bool animated;
    std::unordered_map<std::string, AnimData*> animations;
    std::string currentAnimation;
};

#endif // __TORPEDODATA_H__
