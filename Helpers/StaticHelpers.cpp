#include "StaticHelpers.h"

#include <iostream>

USING_NS_CC;

/// only works for upper case & numbers
char StaticHelpers::keycodeToChar(EventKeyboard::KeyCode code)
{
    int offset = (int)code;
    if(124 <= offset && offset <= 150)
    {
        return 'A'+(offset-124);
    }
    else if(76 <= offset && offset <= 85)
    {
        return '0'+(offset-76);
    }
    else
    {
        return '\0';
    }
}