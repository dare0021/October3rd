#ifndef __O3SPRITE_H__
#define __O3SPRITE_H__

#include "cocos2d.h"
#include "AnimData.h"
#include "Helpers/Consts.h"
#include <unordered_map>

/// All objects on screen, such as the player sub, decoys, or even popup messages
/// Typical values: Mass: hundreds, Friction: Single digit, Force: Thousands
/// If an animated sprite fails to render, check if it is being updated
/// An animated sprite that was never updated will display is original sprite,
/// which can be transparent, too small to notice, or nonexistant to begin with.
class O3Sprite : public cocos2d::Node
{
public:
    O3Sprite(std::string path, bool overrideForcedSpriteInitialization = false);
    ~O3Sprite();
    void update(float);
    cocos2d::Sprite* addSprite(std::string name, std::string path, bool visible = true);
    cocos2d::Sprite* addSprite(std::string name, cocos2d::Sprite* sprite, bool visible = true);
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
    float getNoiseLevel();
    void setMaxSpeed(float);
    float getMaxSpeed();
    void setMaxForce(float);
    float getMaxForce();
    void setTurnSpeed(float);
    float getTurnSpeed();
    void setTargetHeading(float);
    float getTargetHeading();
    cocos2d::Vec2 getHeadingVector();
    bool isTurning();

    std::string getCurrentAnimation();
    void addAnimation(std::string name, std::string path, int count, float frameRate = TARGET_FPS, bool loop = true);
    bool setAnimation(std::string name);
    bool forceAnimation(std::string name);
    bool playAnimation();
    bool isAnimated();
    bool isDone();
    bool stopAnimation();

protected:
    PhysicsModel physicsModel;
    bool updateSuspended;
    float updateSuspendTime;
    ///effectiveForce = force - friction * v^2 / 2
    float friction;
    float speed, mass, force, maxSpeed, maxForce, turnSpeed, targetHeading;
    ///Separate name in O3Sprite from Sprite name
    ///So we can use this name for roles instead of a unique ID
    std::unordered_map<std::string, cocos2d::Sprite*> sprites;

    bool animated;
    std::unordered_map<std::string, AnimData*> animations;
    std::string currentAnimation;

private:
    static int lastID;
    int ID;
};

#endif // __O3SPRITE_H__
