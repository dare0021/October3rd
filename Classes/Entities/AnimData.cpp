#include "AnimData.h"

USING_NS_CC;

AnimData::AnimData(std::string path, int count, float frameRate, bool loop) :
timeOffset(0),
_isLoop(loop),
_isDone(false)
{
	for (int i=0; i<count; i++)
	{
		std::stringstream ss;
		ss << path << "/" << i << ".png";
		data.push_back(new AnimElem(ss.str(), 1.0f / frameRate));
	}
	currentSprite = data.begin();
}

AnimData::AnimData(std::vector<AnimElem*> d, bool loop) :
data(d),
timeOffset(0),
_isLoop(loop),
_isDone(false)
{
	currentSprite = data.begin();
}

AnimData::~AnimData()
{
	for (auto iter : data)
	{
		delete iter;
	}
}

void AnimData::update(float dt)
{
	if(!data.size() || isDone())
		return;
	float x = dt + timeOffset;
	while(x > (*currentSprite)->lifetime)
	{
		x -= (*currentSprite)->lifetime;
		currentSprite = std::next(currentSprite, 1);
		if(currentSprite == data.end())
		{
			if(_isLoop)
				currentSprite = data.begin();
			else
				_isDone = true;
		}
	}
	timeOffset = x;
}

Sprite* AnimData::getCurrentSprite()
{
	return Sprite::create((*currentSprite)->path);
}

int AnimData::getFrameNumber()
{
	return currentSprite - data.begin();
}

bool AnimData::isLoop()
{
	return _isLoop;
}

bool AnimData::isDone()
{
	return _isDone;
}