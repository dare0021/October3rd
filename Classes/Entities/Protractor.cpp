#include "Protractor.h"

USING_NS_CC;

Protractor::Protractor(std::string path, float radius) : O3Sprite(path + "/center.png"),
cursorAngle(-90), radius(radius)
{
	setName("Protractor");
	DrawNode *drawnode = DrawNode::create();
	drawnode->drawDot(Vec2(0,0), radius, Color4F(1,1,1,0.2));
	removeSprite("mainSprite");
	addSprite("mainSprite", (Sprite*)drawnode);

	drawnode = DrawNode::create();
}

void Protractor::setCursorAngle(float nca)
{
	CCASSERT((0 <= nca && nca < 360) || nca == -90, "Invalid angle for Protractor");
	cursorAngle = nca;

	if(getChildByName("indicatorLine"))
		removeChildByName("indicatorLine");
	if(cursorAngle >= 0)
	{
		DrawNode *drawnode = DrawNode::create();
		drawnode->drawSegment(Vec2(0,0), radius*Vec2(sin(cursorAngle*M_PI/180), cos(cursorAngle*M_PI/180)), 
								PROTRACTOR_LINE_THICKNESS, Color4F(1,1,1,0.7));
		addSprite("indicatorLine", (Sprite*)drawnode);
	}
}

float Protractor::getCursorAngle()
{
	return cursorAngle;
}

float Protractor::getRadius()
{
	return radius;
}