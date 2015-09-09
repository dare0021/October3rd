#ifndef __O3SPRITE_H__
#define __O3SPRITE_H__

#include "cocos2d.h"

#include <unordered_map>

///All objects on screen, such as the player sub, decoys, or even popup messages
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
    void setForce(cocos2d::Vec2);
    cocos2d::Vec2 getForce();
    void setSpeed(float);
    float getSpeed();

private:
	static int lastID;
	int ID;
    PhysicsModel physicsModel;
    bool updateSuspended;
    float updateSuspendTime;
    float speed;
    cocos2d::Vec2 force;
    ///Separate name in O3Sprite from Sprite name
    ///So we can use this name for roles instead of a unique ID
    std::unordered_map<std::string, cocos2d::Sprite*> sprites;
};

#endif // __O3SPRITE_H__
