#include "Torpedo.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Torpedo::Torpedo(TorpedoData* prototype, int owner) : O3Sprite("torpedos/"+prototype->spritePath),
ownerID(owner)
{

	physicsModel = prototype->physicsModel;
	friction = prototype->friction;
	speed = prototype->initialSpeed;
	mass = prototype->mass;
	force = prototype->initialForce;
	maxSpeed = prototype->maxSpeed;
	maxForce = prototype->maxForce;
	turnSpeed = prototype->turnSpeed;
	animated = prototype->animated;
	currentAnimation = prototype->initialAnimation;
	animations = std::unordered_map<std::string, AnimData*>(prototype->animations);
	setPosition(prototype->positionOffset);
	setRotation(prototype->headingOffset);
}