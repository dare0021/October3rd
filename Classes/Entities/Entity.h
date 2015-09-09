#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "O3Sprite.h"

///Everything in the playing field, such as the player, decoys, torpedos, etc
class Entity : public O3Sprite
{
public:
	Entity(std::string path);
	float getNoiseLevel();

private:
};

#endif // __ENTITY_H__
