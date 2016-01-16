#include "Submarine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Submarine::Submarine(std::string path) : O3Sprite(path)
{
}

/// returns if an entry was overwritten
bool Submarine::addTorpedoPrototype(TorpedoData* data)
{
	bool out = false;
	if(torpedoPrototypes.find(data->getName()) != torpedoPrototypes.end())
	{
		torpedoPrototypes.erase(data->getName());
		out = true;
	}
	torpedoPrototypes.insert({data->getName(), data});
	return out;
}

bool Submarine::removeTorpedoPrototype(std::string name)
{
	return torpedoPrototypes.erase(name);
}

TorpedoData* Submarine::getTorpedoPrototype(std::string name)
{
	auto out = torpedoPrototypes.find(name);
	if(out == torpedoPrototypes.end())
		return nullptr;
	return out->second;
}

Torpedo* Submarine::spawnTorpedo(std::string name, float targetHeading)
{
	return spawnTorpedo(getTorpedoPrototype(name), targetHeading);
}

Torpedo* Submarine::spawnTorpedo(TorpedoData* prototype, float targetHeading)
{
	CCASSERT(prototype, "nullptr");

	auto torpedo = new Torpedo(prototype, getID());
	torpedo->setPosition(torpedo->getPosition() + getPosition());
	torpedo->setRotation(torpedo->getRotation() + getRotation());
	torpedo->setTargetHeading(targetHeading);
	torpedo->setSpeed(torpedo->getSpeed() + getSpeed());
	return torpedo;
}