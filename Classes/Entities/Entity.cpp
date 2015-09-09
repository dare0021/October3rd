#include "Entity.h"

USING_NS_CC;

Entity::Entity(std::string path) : O3Sprite(path)
{

}

float Entity::getNoiseLevel()
{
	return -1;
}