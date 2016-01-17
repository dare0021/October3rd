#include "TorpedoData.h"
#include "ui/CocosGUI.h"
#include "Helpers/Consts.h"

#include <iostream>
#include <fstream>

USING_NS_CC;

JLDDoc* TorpedoData::torpedoPrototypes = NULL;

void TorpedoData::init(std::string filepath)
{
	auto path = DATA_PATH + filepath;
	CCASSERT(FileUtils::getInstance()->isFileExist(path), "No such file");
	torpedoPrototypes = JLDIO::parseText(FileUtils::getInstance()->getStringFromFile(path));
}

TorpedoData::TorpedoData(std::string name) : 
name(name), 
physicsModel(PhysicsModel::SpeedOnly),
spritePath("NOT_INITIALIZED"), 
friction(0), 
initialSpeed(0), 
maxSpeed(-1),
mass(1),
initialForce(0),
maxForce(-1),
turnSpeed(0),
headingOffset(0),
animated(false),
positionOffset(Vec2::ZERO),
initialAnimation("NOT_INITIALIZED"),
animations(std::unordered_map<std::string, AnimData*>())
{
}

/// Class must be initialized before using this function
TorpedoData::TorpedoData(std::string name, std::string prototypeName)
 : TorpedoData(name)
{
	CCASSERT(torpedoPrototypes, "Class was not initialized");
	JLDDoc* prototype = (JLDDoc*)(torpedoPrototypes->getValue(prototypeName));
	JLDData* data = NULL;
	if(data = prototype->getValue("physicsModel"))
		this->physicsModel = (PhysicsModel)std::stoi(((JLDString*)data)->getValue());
	if(data = prototype->getValue("spritePath"))
		this->spritePath = ((JLDString*)data)->getValue();
	if(data = prototype->getValue("friction"))
		this->friction = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("initialSpeed"))
		this->initialSpeed = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("maxSpeed"))
		this->maxSpeed = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("mass"))
		this->mass = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("initialForce"))
		this->initialForce = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("maxForce"))
		this->maxForce = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("turnSpeed"))
		this->turnSpeed = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("headingOffset"))
		this->headingOffset = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("animated"))
		this->animated = std::stof(((JLDString*)data)->getValue());
	if(data = prototype->getValue("positionOffset"))
		this->positionOffset = Vec2(std::stof(((JLDString*)(((JLDList*)data)->getValue(0)))->getValue()),
									 std::stof(((JLDString*)(((JLDList*)data)->getValue(1)))->getValue()));
}

std::string TorpedoData::getName()
{
	return name;
}