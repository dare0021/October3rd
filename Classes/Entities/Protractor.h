#ifndef __PROTRACTOR_H__
#define __PROTRACTOR_H__

#include "cocos2d.h"
#include "O3Sprite.h"

class Protractor : public O3Sprite
{
public:
	///folder containing the resource files
	Protractor(std::string path, float radius);

	void setCursorAngle(float);
	float getCursorAngle();
	float getRadius();

private:
	float cursorAngle, radius;
};

#endif // __PROTRACTOR_H__
