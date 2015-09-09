#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "O3Sprite.h"

///Everything in the playing field, such as the player, decoys, torpedos, etc
class Entity : O3Sprite
{
public:
	Entity();
	float getNoiseLevel();

private:
	float speed;
	cocos2d::Vec2 force;
};

#endif // __ENTITY_H__
