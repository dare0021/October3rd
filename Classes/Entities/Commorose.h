#ifndef __COMMOROSE_H__
#define __COMMOROSE_H__

#include "cocos2d.h"
#include "O3Sprite.h"

class Commorose : public O3Sprite
{
public:
	///folder containing the resource files
	Commorose(std::string path);

	void setCursorAngle(float);
	float getCursorAngle();
	///Returns which item is on, with 0 being the top right one going CW
	int getMode();

private:
	float cursorAngle;
};

#endif // __COMMOROSE_H__
