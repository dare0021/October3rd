#include "O3Sprite.h"

#include "Helpers/Consts.h"

USING_NS_CC;

int O3Sprite::lastID = -1;

O3Sprite::O3Sprite(std::string mainSprite) :
ID(lastID+1),
physicsModel(Stationary),
speed(0),
friction(0),
mass(1),
force(0),
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

void O3Sprite::setForce(float nf)
{
	force = nf;
}

float O3Sprite::getForce()
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

void O3Sprite::setMass(float nm)
{
	mass = nm;
}

float O3Sprite::getMass()
{
	return mass;
}

void O3Sprite::setFriction(float nfk)
{
	friction = nfk;
}

float O3Sprite::getFriction()
{
	return friction;
}


Vec2 O3Sprite::getHeadingVector()
{
	Vec2 vect = Vec2(0, 1);
	vect.rotate(Vec2::ZERO, -1 * getRotation() * M_PI / 180);
	return vect;
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

	if(physicsModel == Newtonian)
	{
		auto mu = speed>0 ? friction : -1 * friction;
		speed += (force - mu * speed * speed / 2) * dt / mass;
	}
	if(physicsModel != Stationary)
	{
		Vec2 moveBy = getHeadingVector() * speed * dt;
		setPosition(getPosition() + moveBy);
	}
	CCLOG("FORCE %f \t SPD %f", force, speed);
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
	sprites.erase(name);
	return found;
}