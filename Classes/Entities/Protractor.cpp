#include "Protractor.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Protractor::Protractor(std::string path, float radius) : O3Sprite(path + "/center.png"),
cursorAngle(-90), radius(radius)
{
	setName("Protractor");
	DrawNode *drawnode = DrawNode::create();
	drawnode->drawDot(Vec2(0,0), radius, Color4F(1,1,1,0.1));
	for (int i=0; i<360; i+=10)
	{
		drawnode->drawSegment(radius*0.9*angleToVector(i), radius*angleToVector(i), 
								PROTRACTOR_LINE_THICKNESS, Color4F(1,1,1,0.2));
		if (i % 20)
			continue;
		std::stringstream ss;
		ss << i;
		auto txt = ui::Text::create(ss.str(), "", 15);
		txt->setPosition(radius*0.8*angleToVector(i));
		txt->setOpacity(128);
		drawnode->addChild(txt);
	}
	removeSprite("mainSprite");
	addSprite("mainSprite", (Sprite*)drawnode);
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
		drawnode->drawSegment(Vec2(0,0), radius*angleToVector(cursorAngle), 
								PROTRACTOR_LINE_THICKNESS, Color4F(1,1,1,0.5));
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

Vec2 Protractor::angleToVector(float a)
{
	return Vec2(sin(a*M_PI/180), cos(a*M_PI/180));
}