#ifndef __O3SPRITE_H__
#define __O3SPRITE_H__

#include "cocos2d.h"

#include <unordered_map>

///All objects on screen, such as the player sub, decoys, or even popup messages
///Typical values: Mass: hundreds, Friction: Single digit, Force: Thousands
class O3Sprite : public cocos2d::Node
{
public:
    enum PhysicsModel
    {
        Stationary,
        SpeedOnly,
        Newtonian
    };

    O3Sprite(std::string path);
    ~O3Sprite();
    void update(float);
    void addSprite(std::string name, std::string path);
    bool removeSprite(std::string name);

    int getID();
    void setPhysicsModel(PhysicsModel);
    PhysicsModel getPhysicsModel();
    void setForce(float);
    float getForce();
    void setSpeed(float);
    float getSpeed();
    void setMass(float);
    float getMass();
    void setFriction(float);
    float getFriction();
    cocos2d::Vec2 getHeadingVector();

private:
	static int lastID;
	int ID;
    PhysicsModel physicsModel;
    bool updateSuspended;
    float updateSuspendTime;
    ///effectiveForce = force - friction * v^2 / 2
    float friction;
    float speed, mass, force;
    ///Separate name in O3Sprite from Sprite name
    ///So we can use this name for roles instead of a unique ID
    std::unordered_map<std::string, cocos2d::Sprite*> sprites;
};

#endif // __O3SPRITE_H__
