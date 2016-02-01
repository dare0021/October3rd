#include "Submarine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Submarine::Submarine(std::string path) : O3Sprite(path),
hp(100),
maxHP(100),
turningForce(2000)
{
}

/// returns if an entry was overwritten
/// overwritten entries are freed
bool Submarine::addTorpedoPrototype(TorpedoData* data)
{
	bool out = false;
	if(torpedoPrototypes.find(data->getName()) != torpedoPrototypes.end())
	{
		removeTorpedoPrototype(data->getName());
		out = true;
	}
	torpedoPrototypes.insert({data->getName(), data});
	return out;
}

/// frees said prototype
bool Submarine::removeTorpedoPrototype(std::string name)
{
	delete torpedoPrototypes.find(name)->second;
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
	torpedo->setSpeed(torpedo->getSpeed());
	return torpedo;
}

void Submarine::setHP(float nhp)
{
	hp = nhp;
}

float Submarine::getHP()
{
	return hp;
}

void Submarine::setMaxHP(float nhp)
{
	maxHP = nhp;
}

float Submarine::getMaxHP()
{
	return maxHP;
}

void Submarine::setTurningForce(float nv)
{
	turningForce = nv;
}

float Submarine::getTurningForce()
{
	return turningForce;
}