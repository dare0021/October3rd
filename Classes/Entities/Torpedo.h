#ifndef __TORPEDO_H__
#define __TORPEDO_H__

#include "cocos2d.h"
#include "O3Sprite.h"
#include "TorpedoData.h"

class Torpedo : public O3Sprite
{
public:
	Torpedo(TorpedoData* prototype, int owner);

	int getOwnerID();

private:
	int ownerID;
};

#endif // __TORPEDO_H__
