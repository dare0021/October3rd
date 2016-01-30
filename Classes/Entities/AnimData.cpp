#include "AnimData.h"

USING_NS_CC;

AnimData::AnimData(std::string path, int count, float frameRate, bool loop) :
timeOffset(0),
_isLoop(loop),
_isDone(false),
lastAccessedName("NONE")
{
	for (int i=0; i<count; i++)
	{
		data.push_back(new AnimElem(
			path + "/" + std::to_string(i) + ".png", 1.0f / frameRate));
	}
	currentElem = data.begin();
	lastAccessedName = getCurrentName();
}

AnimData::AnimData(std::vector<AnimElem*> d, bool loop) :
data(d),
timeOffset(0),
_isLoop(loop),
_isDone(false)
{
	currentElem = data.begin();
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
	while(x > (*currentElem)->lifetime)
	{
		x -= (*currentElem)->lifetime;
		currentElem = std::next(currentElem, 1);
		if(currentElem == data.end())
		{
			if(_isLoop)
				currentElem = data.begin();
			else
			{
				currentElem = std::prev(currentElem, 1);
				_isDone = true;
				break;
			}
		}
	}
	timeOffset = x;
}

std::string AnimData::getLastAccessedName()
{
	return lastAccessedName;
}

std::string AnimData::getCurrentName()
{
	lastAccessedName = (*currentElem)->name;
	return (*currentElem)->name;
}

int AnimData::getFrameNumber()
{
	return currentElem - data.begin();
}

bool AnimData::isLoop()
{
	return _isLoop;
}

bool AnimData::isDone()
{
	return _isDone;
}
