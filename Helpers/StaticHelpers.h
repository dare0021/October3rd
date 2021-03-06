#ifndef __STATICHELPERS_H__
#define __STATICHELPERS_H__

#include "cocos2d.h"

class StaticHelpers
{
public:
	static cocos2d::Sprite* duplicateSprite(cocos2d::Sprite *sprite);
    static char keycodeToChar(cocos2d::EventKeyboard::KeyCode code);
    static float headingAngle(cocos2d::Vec2 vect);
    static bool angleIsCloserToB(float a, float val, float b);
    static cocos2d::Vec2 degreesToVec2(float degrees);
};

#endif // __STATICHELPERS_H__
