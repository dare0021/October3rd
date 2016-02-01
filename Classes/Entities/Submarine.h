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

	void setHP(float);
	float getHP();
	void setMaxHP(float);
	float getMaxHP();
	/// Only for noise calculation. There is no simulation for
	/// physics based turning.
	/// If the sub is turning, this is added to the normal force if
	/// accessed via getForceWithTurningForce()
	void setTurningForce(float);
	float getTurningForce();

private:
	std::unordered_map<std::string, TorpedoData*> torpedoPrototypes;
	float hp, maxHP, turningForce;
};

#endif // __SUBMARINE_H__
