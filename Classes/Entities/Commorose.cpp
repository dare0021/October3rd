#include "Commorose.h"

USING_NS_CC;

Commorose::Commorose(std::string path) : O3Sprite(path + "/center.png"),
cursorAngle(-90)
{
	setName("commorose");
	addSprite("defences off", path + "/defences.png")->setPosition(130, -35);
	addSprite("ping off", path + "/ping.png")->setPosition(-130, -35);
	addSprite("speed off", path + "/speed.png")->setPosition(130, 35);
	addSprite("weapons off", path + "/weapons.png")->setPosition(-130, 35);
	addSprite("defences on", path + "/defences-on.png", false)->setPosition(130, -35);
	addSprite("ping on", path + "/ping-on.png", false)->setPosition(-130, -35);
	addSprite("speed on", path + "/speed-on.png", false)->setPosition(130, 35);
	addSprite("weapons on", path + "/weapons-on.png", false)->setPosition(-130, 35);
}

void Commorose::setCursorAngle(float nca)
{
	CCASSERT((0 <= nca && nca < 360) || nca == -90, "Invalid angle for Commorose");
	cursorAngle = nca;

	switch (getMode())
	{
		case -1:
		{
			sprites.find("defences on")->second->setVisible(false);
			sprites.find("ping on")->second->setVisible(false);
			sprites.find("speed on")->second->setVisible(false);
			sprites.find("weapons on")->second->setVisible(false);
			break;
		}
		case 0:
		{
			sprites.find("defences on")->second->setVisible(false);
			sprites.find("ping on")->second->setVisible(false);
			sprites.find("speed on")->second->setVisible(true);
			sprites.find("weapons on")->second->setVisible(false);
			break;
		}
		case 1:
		{
			sprites.find("defences on")->second->setVisible(true);
			sprites.find("ping on")->second->setVisible(false);
			sprites.find("speed on")->second->setVisible(false);
			sprites.find("weapons on")->second->setVisible(false);
			break;
		}
		case 2:
		{
			sprites.find("defences on")->second->setVisible(false);
			sprites.find("ping on")->second->setVisible(true);
			sprites.find("speed on")->second->setVisible(false);
			sprites.find("weapons on")->second->setVisible(false);
			break;
		}
		case 3:
		{
			sprites.find("defences on")->second->setVisible(false);
			sprites.find("ping on")->second->setVisible(false);
			sprites.find("speed on")->second->setVisible(false);
			sprites.find("weapons on")->second->setVisible(true);
			break;
		}
	}
}

float Commorose::getCursorAngle()
{
	return cursorAngle;
}

int Commorose::getMode()
{
	return ((int)cursorAngle)/90;
}