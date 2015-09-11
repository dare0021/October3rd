#ifndef __STATICHELPERS_H__
#define __STATICHELPERS_H__

#include "cocos2d.h"

class StaticHelpers
{
public:
    static char keycodeToChar(cocos2d::EventKeyboard::KeyCode code);
    static float StaticHelpers::headingAngle(cocos2d::Vec2 vect);
};

#endif // __STATICHELPERS_H__
