#include "TorpedoData.h"
#include "ui/CocosGUI.h"
#include "Helpers/Consts.h"

USING_NS_CC;

TorpedoData::TorpedoData(std::string name) : 
name(name), 
physicsModel(PhysicsModel::Newtonian),
spritePath("NOT_INITIALIZED"), 
friction(0), 
initialSpeed(0), 
maxSpeed(-1),
mass(1),
initialForce(0),
maxForce(-1),
turnSpeed(10),
headingOffset(0),
animated(false),
positionOffset(Vec2::ZERO),
initialAnimation("NOT_INITIALIZED"),
animations(std::unordered_map<std::string, AnimData*>())
{
}

std::string TorpedoData::getName()
{
	return name;
}