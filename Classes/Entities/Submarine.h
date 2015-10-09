#ifndef __SUBMARINE_H__
#define __SUBMARINE_H__

#include "cocos2d.h"
#include "O3Sprite.h"
#include "Torpedo.h"

class Submarine : public O3Sprite
{
public:
	Submarine(std::string path);

	void addTorpedoPrototype(TorpedoData*);
	TorpedoData* getTorpedoPrototype(std::string name);
	Torpedo* spawnTorpedo(std::string name);
	Torpedo* spawnTorpedo(TorpedoData*);

private:
	std::unordered_map<std::string, TorpedoData*> torpedoPrototypes;
};

#endif // __SUBMARINE_H__
