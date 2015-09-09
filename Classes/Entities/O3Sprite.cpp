#include "O3Sprite.h"

USING_NS_CC;

int O3Sprite::lastID = -1;

O3Sprite::O3Sprite(std::string mainSprite) :
ID(lastID+1),
physicsModel(Stationary),
speed(0),
force(Vec2::ZERO),
updateSuspended(false),
updateSuspendTime(0)
{
	lastID++;
	addSprite("mainSprite", mainSprite);
	setName(mainSprite);
}

O3Sprite::~O3Sprite()
{
	for (auto s : sprites)
	{
		delete s.second;
	}
}

void O3Sprite::setPhysicsModel(PhysicsModel npm)
{
	physicsModel = npm;
}

O3Sprite::PhysicsModel O3Sprite::getPhysicsModel()
{
	return physicsModel;
}

void O3Sprite::setForce(Vec2 nf)
{
	force = nf;
}

Vec2 O3Sprite::getForce()
{
	return force;
}

void O3Sprite::setSpeed(float ns)
{
	speed = ns;
}

float O3Sprite::getSpeed()
{
	return speed;
}

void O3Sprite::update(float dt)
{
	if(updateSuspended)
	{
		updateSuspendTime += dt;
		return;
	}
	float totalDT = dt + updateSuspendTime;
	updateSuspendTime = 0;

	Vec2 moveBy = Vec2(0, speed*dt);
	moveBy.rotate(Vec2::ZERO, -1 * getRotation() * M_PI / 180);
	setPosition(getPosition() + moveBy);
}

void O3Sprite::addSprite(std::string name, std::string path)
{
	auto ms = Sprite::create(path);
	ms->setName(name);
	sprites.insert({ name, ms });
	this->addChild(ms);
}

bool O3Sprite::removeSprite(std::string name)
{
	auto sprite = sprites.find(name)->second;
	bool found = sprites.find(name) != sprites.end();
	this->removeChild(sprite);
	bool erased = sprites.erase(name);
	CCASSERT(found == erased, "found != erased");
	return erased;
}