#ifndef __STATICHELPERS_H__
#define __STATICHELPERS_H__

#include "cocos2d.h"

class StaticHelpers
{
public:
    static std::string Vec3toString(cocos2d::Vec3);
    static std::string Vec2toString(cocos2d::Vec2);
    static char keycodeToChar(cocos2d::EventKeyboard::KeyCode code);
};

#endif // __STATICHELPERS_H__
