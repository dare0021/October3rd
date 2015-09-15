#include "StaticHelpers.h"

#include <iostream>

USING_NS_CC;

///selectively copies parts of the given Sprite
Sprite* StaticHelpers::duplicateSprite(Sprite *sprite)
{
    Sprite* pRet = Sprite::createWithTexture(sprite->getTexture());
    if (sprite->getChildrenCount() > 0) {
        auto children = sprite->getChildren();
        for (auto child : children) {
            auto spriteChild = dynamic_cast<Sprite*>(child);
            if (! spriteChild) continue;
            auto clone = duplicateSprite(spriteChild);
            // can't set boundingBox
            //might need to set textureRect, scale, rotation
            //clone->boundingBox() = spriteChild->boundingBox();
            clone->setContentSize(spriteChild->getContentSize());
            clone->setPosition(spriteChild->getPosition());
            clone->setAnchorPoint(spriteChild->getAnchorPoint());
            clone->setLocalZOrder(spriteChild->getLocalZOrder());
            clone->setTag(spriteChild->getTag());
            pRet->addChild(clone);
        }
    }
    return pRet;
}

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

/// translates vector to degrees with due north as 0 and the angle going CW
float StaticHelpers::headingAngle(Vec2 vect)
{
    float retval = atan(vect.x/vect.y);
    if(vect.y < 0)
        retval += M_PI;
    if(retval < 0)
        retval += 2 * M_PI;
    return retval * 180 / M_PI;
}