#include "Protractor.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

/// Dummy sprite is not used. Just so I don't have to rewrite O3Sprite
Protractor::Protractor(std::string path, float radius) : O3Sprite(path + "/1x1empty.png"),
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
		auto txt = ui::Text::create(std::to_string(i), "", PROTRACTOR_LABEL_SIZE);
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