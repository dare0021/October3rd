#include "O3Sprite.h"
#include "Helpers/StaticHelpers.h"

USING_NS_CC;

int O3Sprite::lastID = -1;

O3Sprite::O3Sprite(std::string mainSprite, bool overrideForcedSpriteInitialization) :
ID(lastID+1),
physicsModel(PhysicsModel::Stationary),
speed(0),
maxSpeed(-1),
friction(0),
mass(1),
force(0),
maxForce(-1),
turnSpeed(10),
targetHeading(0),
updateSuspended(false),
updateSuspendTime(0),
animated(false),
currentAnimation("NOT_INITIALIZED")
{
	lastID++;
	if(!overrideForcedSpriteInitialization)
	{
		addSprite("mainSprite", mainSprite);
	}
	setCascadeColorEnabled(true);
	setCascadeOpacityEnabled(true);
	setName(mainSprite);
}

O3Sprite::~O3Sprite()
{
	for (auto s : sprites)
	{
		delete s.second;
	}
}

int O3Sprite::getID()
{
	return ID;
}

void O3Sprite::setPhysicsModel(PhysicsModel npm)
{
	physicsModel = npm;
}

PhysicsModel O3Sprite::getPhysicsModel()
{
	return physicsModel;
}

/// maxForce is ignored if maxForce < 0
void O3Sprite::setForce(float nf)
{
	if(maxForce < 0)
	{
		force = nf;
		return;
	}
	force = nf > maxForce ? maxForce : nf;
	force = force < -1*maxForce ? -1*maxForce : force;

	CCLOG("NEW FORCE %f, MAX %f", force, maxForce);
}

float O3Sprite::getForce()
{
	return force;
}

/// maxSpeed is ignored if maxSpeed < 0
void O3Sprite::setSpeed(float ns)
{
	if(maxSpeed < 0)
	{
		speed = ns;
		return;
	}
	speed = ns > maxSpeed ? maxSpeed : ns;
	speed = speed < -1*maxSpeed ? -1*maxSpeed : speed;
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

float O3Sprite::getNoiseLevel()
{
	CCASSERT(0, "Not implemented");
	return -1;
}

void O3Sprite::setMaxSpeed(float nv)
{
	maxSpeed = nv;
}

float O3Sprite::getMaxSpeed()
{
	return maxSpeed;
}

void O3Sprite::setMaxForce(float nv)
{
	maxForce = nv;
}

float O3Sprite::getMaxForce()
{
	return maxForce;
}

void O3Sprite::setTurnSpeed(float nv)
{
	turnSpeed = nv;
}

float O3Sprite::getTurnSpeed()
{
	return turnSpeed;
}

void O3Sprite::setTargetHeading(float nv)
{
	targetHeading = nv;
}

float O3Sprite::getTargetHeading()
{
	return targetHeading;
}

Vec2 O3Sprite::getHeadingVector()
{
	Vec2 vect = Vec2(0, 1);
	vect.rotate(Vec2::ZERO, -1 * getRotation() * M_PI / 180);
	return vect;
}

bool O3Sprite::isTurning()
{
	float diff = getRotation() - getTargetHeading();
	diff = diff < 0 ? diff * -1 : diff;
	return (diff > ROTATION_EQUALITY_THRESHOLD) && 
			(getTurnSpeed() > ROTATION_EQUALITY_THRESHOLD);
	// checking if the sub can turn, since wanting to turn does
	// dot neccessarily mean turning
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

	if(physicsModel == PhysicsModel::Newtonian)
	{
		auto mu = speed>0 ? friction : -1 * friction;
		setSpeed(speed + (dt / mass) * (force - mu * speed * speed / 2));
	}
	if(physicsModel != PhysicsModel::Stationary)
	{
		Vec2 moveBy = getHeadingVector() * speed * dt;
		setPosition(getPosition() + moveBy);
	}
	if(getRotation() != targetHeading)
	{
		float dh = targetHeading - getRotation();
		dh = dh>180 ? dh-360 : dh;
		auto effectiveSpeed = turnSpeed * dt;
		dh = dh<-1*effectiveSpeed ? -1*effectiveSpeed : dh;
		dh = dh>effectiveSpeed ? effectiveSpeed : dh;
		setRotation(getRotation() + dh);
	}

	while(animated && animations.size())
	{
		for (auto kvp : animations)
			kvp.second->update(dt);
		auto ad = animations.find(currentAnimation);
		if(ad == animations.end())
			break;
		getChildByName(ad->second->getLastAccessedName())->setVisible(false);
		getChildByName(ad->second->getCurrentName())->setVisible(true);
		break;
	}
}

Sprite* O3Sprite::addSprite(std::string name, std::string path, bool visible)
{
	auto ms = Sprite::create(path);
	ms->setName(name);
	ms->setVisible(visible);
	sprites.insert({ name, ms });
	this->addChild(ms);
	return ms;
}

Sprite* O3Sprite::addSprite(std::string name, Sprite* sprite, bool visible)
{
	sprite->setName(name);
	sprite->setVisible(visible);
	sprites.insert({ name, sprite });
	this->addChild(sprite);
	return sprite;
}

bool O3Sprite::removeSprite(std::string name)
{
	auto sprite = sprites.find(name)->second;
	bool found = sprites.find(name) != sprites.end();
	this->removeChild(sprite);
	sprites.erase(name);
	return found;
}

std::string O3Sprite::getCurrentAnimation()
{
	return currentAnimation;
}

void O3Sprite::addAnimation(std::string name, std::string path, int count, float frameRate, bool loop)
{
	auto val = new AnimData(path, count, frameRate, loop);
	for (int i=0; i<count; i++)
	{
		std::string filePath = path + "/" + std::to_string(i) + ".png";
		addSprite(filePath, Sprite::create(filePath), false);
	}
	animations.insert({ name, val });
}

bool O3Sprite::setAnimation(std::string name)
{
	if(currentAnimation != name)
		return forceAnimation(name);
	return false;
}

bool O3Sprite::forceAnimation(std::string name)
{
	auto ad = animations.find(name);
	CCASSERT(ad != animations.end(), "Non-existant animation name");
	if(ad == animations.end()) //assert does not run on release builds
		return false;
	currentAnimation = name;
	return true;
}

bool O3Sprite::playAnimation()
{
	bool output = !animated;
	animated = true;
	return output;
}

bool O3Sprite::isAnimated()
{
	return animated;
}

bool O3Sprite::isDone()
{
	return animations.find(getCurrentAnimation())->second->isDone();
}

bool O3Sprite::stopAnimation()
{
	bool output = animated;
	animated = false;
	return output;
}