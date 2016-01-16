#ifndef __SUBMARINE_H__
#define __SUBMARINE_H__

#include "cocos2d.h"
#include "O3Sprite.h"
#include "Torpedo.h"

class Submarine : public O3Sprite
{
public:
	Submarine(std::string path);

	bool addTorpedoPrototype(TorpedoData*);
	bool removeTorpedoPrototype(std::string name);
	TorpedoData* getTorpedoPrototype(std::string name);
	Torpedo* spawnTorpedo(std::string name, float targetHeading);
	Torpedo* spawnTorpedo(TorpedoData*, float targetHeading);

private:
	std::unordered_map<std::string, TorpedoData*> torpedoPrototypes;
};

#endif // __SUBMARINE_H__
