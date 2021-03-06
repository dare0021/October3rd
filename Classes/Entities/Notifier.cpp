#include "Notifier.h"
#include "Helpers/Consts.h"
#include "Helpers/StaticHelpers.h"

//#define SHOW_OFFSCREEN_OBJECTS

USING_NS_CC;

Notifier::Notifier(std::string path, Vec2 screenSize) : O3Sprite("", true),
resourceFolderPath(path),
timeSinceLastOpacityUpdate(0),
screenSize(screenSize)
{
	// need to attach the prototypes to something to prevent
	// the garbage collector from freeing them
	auto prototypes = Sprite::create();

	offscreenEnemyPrototype = Sprite::create(resourceFolderPath + "/offscreen/enemy-bg.png");
	auto offscreenEnemyFG = Sprite::create(resourceFolderPath + "/offscreen/enemy-fg.png");
	offscreenEnemyPrototype->addChild(offscreenEnemyFG);
	offscreenEnemyFG->setPosition(Vec2(20, 20));
	prototypes->addChild(offscreenEnemyPrototype);

	offscreenTorpedoPrototype = Sprite::create(resourceFolderPath + "/offscreen/torpedo-bg.png");
	auto offscreenTorpedoFG = Sprite::create(resourceFolderPath + "/offscreen/torpedo-fg.png");
	offscreenTorpedoPrototype->addChild(offscreenTorpedoFG);
	offscreenTorpedoFG->setPosition(Vec2(20, 20));
	prototypes->addChild(offscreenTorpedoPrototype);

	offscreenCMPrototype = Sprite::create(resourceFolderPath + "/offscreen/cm.png");
	prototypes->addChild(offscreenCMPrototype);
	offscreenPingPrototype = Sprite::create(resourceFolderPath + "/offscreen/ping.png");
	prototypes->addChild(offscreenPingPrototype);
	offscreenTubeFillingPrototype = Sprite::create(resourceFolderPath + "/offscreen/tube-filling.png");
	prototypes->addChild(offscreenTubeFillingPrototype);

	prototypes->setVisible(false);
	addChild(prototypes);


	minimap = Sprite::create(resourceFolderPath + "/bg-big.png");
    Vec2 minimapPos = screenSize/2 - MINIMAP_SIZE/2;
    minimapPos.y *= -1;
    minimap->setPosition(minimapPos);
    addChild(minimap);

	
	auto s = addSprite("hp bar", "centergui/hp.png");
	s->setOpacity(255*.3);
	s->setPosition(Vec2(-212 , -0.5*GUI_BAR_HEIGHT));
	s->setAnchorPoint(Vec2(0.5, 0));
	s = addSprite("noise bar", "centergui/noise.png");
	s->setOpacity(255*.4);
	s->setPosition(Vec2(225, -0.5*GUI_BAR_HEIGHT));
	s->setAnchorPoint(Vec2(0.5, 0));
	s = addSprite("thrust bar", "centergui/thrust.png");
	s->setOpacity(255*.5);
	s->setPosition(Vec2(212, -0.5*GUI_BAR_HEIGHT));
	s->setAnchorPoint(Vec2(0.5, 0));
	s = addSprite("subsystem menu", "centergui/subsystem-menu.png");
	s->setOpacity(255*.5);
	s->setPosition(Vec2(-85, -230));
	// static sprite ergo no need to fudge with the anchor point


	hpText = Label::createWithTTF("72", "fonts/NanumGothic.ttf", 33);
	hpText->setPosition(Vec2(-220, -140));
	hpText->setAnchorPoint(Vec2(1, 0.5));
	hpText->setColor(Color3B(0, 255, 102));
	hpText->setOpacity(255*.5);
	addChild(hpText);
	speedText = Label::createWithTTF("12 kt", "fonts/NanumGothic.ttf", 37);
	speedText->setPosition(Vec2(-152, -188));
	speedText->setAnchorPoint(Vec2(1, 0.5));
	speedText->setColor(Color3B(0, 255, 255));
	speedText->setOpacity(255*.5);
	addChild(speedText);
	thrustText = Label::createWithTTF("0%", "fonts/NanumGothic.ttf", 27);
	thrustNormalX = 172;
	thrustNegativeX = 200;
	thrustText->setPosition(Vec2(thrustNormalX, -177));
	thrustText->setAnchorPoint(Vec2(0, 0.5));
	thrustText->setOpacity(255*.5);
	addChild(thrustText);
	noiseText = Label::createWithTTF("0 dB", "fonts/NanumGothic.ttf", 27);
	noiseText->setPosition(Vec2(235, -150));
	noiseText->setAnchorPoint(Vec2(0, 0.5));
	noiseText->setColor(Color3B(255, 0, 0));
	noiseText->setOpacity(255*.7);
	addChild(noiseText);
	menuText = Label::createWithTTF("0: stop\n\n2: full ahead", "fonts/NanumGothicCoding.ttf", 27);
	menuText->setPosition(Vec2(-124, -283));
	menuText->setAnchorPoint(Vec2(0, 0.5));
	menuText->setOpacity(255*.5);
	addChild(menuText);
	activeMenuText = Label::createWithTTF("\n1: half ahead\n", "fonts/NanumGothicCoding.ttf", 27);
	activeMenuText->setPosition(Vec2(-124, -283));
	activeMenuText->setAnchorPoint(Vec2(0, 0.5));
	activeMenuText->setColor(Color3B(255, 0, 0));
	activeMenuText->setOpacity(255*.7);
	addChild(activeMenuText);
}

/// uses the given sprite if this is not isDotNode
/// uses the color if this isDotNode
void Notifier::newMinimapEntry(O3Sprite* minimapSprite,
	Vec2 pos, float ttl, bool isDotNode, int id, std::string dotPath)
{
	#ifndef SHOW_OFFSCREEN_OBJECTS
	// ignore objects outside of bounds	
	// necessary since culling should be done off screen instead of having
	// the torpedo disappear magically
	if(pos.x < -1*GAME_SIZE.x/2 || pos.x > GAME_SIZE.x/2 ||
		pos.y < -1*GAME_SIZE.y/2 || pos.y > GAME_SIZE.y/2)
	{
		auto kvp = minimapEntries.find(id);
		if(kvp != minimapEntries.end())
		{
			kvp->second->fading = true;
		}
		return;
	}
	#endif
	auto kvp = minimapEntries.find(id);
	if(kvp == minimapEntries.end())
	{
		if(isDotNode)
		{
			for (int i=0; i<MINIMAP_DOT_ANIM_COUNT; i++)
			{
				std::string filePath = resourceFolderPath + "/dots/" + dotPath + std::to_string(i) + ".png";
				minimapSprite->addSprite(filePath, filePath);
			}
		}
		minimapEntries[id] = new MinimapElem(minimapSprite, ttl, isDotNode);
		minimap->addChild(minimapSprite);
	}
	else
	{
		// cocos2d::Node anchor is centered
		// cocos2d::Sprite anchor is bottom left
		float xrat = GAME_SIZE.x / MINIMAP_INTERNAL_SIZE.x;
		float yrat = GAME_SIZE.y / MINIMAP_INTERNAL_SIZE.y;
		auto prospectiveVal = Vec2(pos.x / xrat, pos.y / yrat) + MINIMAP_SIZE/2;
		int dx = kvp->second->nextPos.x - prospectiveVal.x;
		int dy = kvp->second->nextPos.y - prospectiveVal.y;
		if(dx || dy)
		{
			kvp->second->nextPos = prospectiveVal;
			kvp->second->dirty = true;
		}
		kvp->second->ttl = ttl;
	}
}

void Notifier::newMinimapTorpedo(Vec2 pos, int id)
{
	auto minimapSprite = new O3Sprite("", true);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, id, "white/");
}

/// AI player
void Notifier::newMinimapSubmarine(Vec2 pos, int id)
{
	auto minimapSprite = new O3Sprite("", true);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, id, "red/");
}

/// the human player
void Notifier::newMinimapPlayer(Vec2 pos, int id)
{
	auto minimapSprite = new O3Sprite("", true);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, id, "green/");
}

void Notifier::newMinimapCounterMeasure(Vec2 pos, int id)
{
	auto minimapSprite = new O3Sprite("", true);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, id, "cyan/");
}

void Notifier::newMinimapPing(Vec2 pos, int id)
{
	auto minimapSprite = new O3Sprite("", true);
	int count = 150;
	int fps = 30;
	minimapSprite->addAnimation("idle", resourceFolderPath + "/ping", count, fps);
	minimapSprite->setAnimation("idle");
	minimapSprite->playAnimation();
	newMinimapEntry(minimapSprite, pos, count / fps, false, id, "NOSUCHPATH");
}

void Notifier::newMinimapTubeFilling(Vec2 pos, int id)
{
	auto minimapSprite = new O3Sprite("", true);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, id, "orange/");
}

void Notifier::newOffscreenEntry(Sprite* offscreenPrototype, 
	Vec2 pos, int id)
{
	#ifndef SHOW_OFFSCREEN_OBJECTS
	// ignore objects outside of bounds
	// necessary since culling should be done off screen instead of having
	// the torpedo disappear magically
	if(pos.x < -1*GAME_SIZE.x/2 || pos.x > GAME_SIZE.x/2 ||
		pos.y < -1*GAME_SIZE.y/2 || pos.y > GAME_SIZE.y/2)
	{
		auto kvp = offscreenEntries.find(id);
		if(kvp != offscreenEntries.end())
		{
			removeChild(kvp->second->sprite, true);
			delete offscreenEntries.find(id)->second;
			offscreenEntries.erase(id);
		}
		return;
	}
	#endif

	bool createNewMinimapOffscreenNotification = false;
	auto oe = offscreenEntries.find(id);
	int currentDirection = isOffscreen(pos);
	int angle = (currentDirection - 1) * 45;
	if(oe != offscreenEntries.end())
	{
		auto entry = oe->second;
		if (entry->direction != currentDirection)
		{
			removeChild(entry->sprite);
			delete entry;
			offscreenEntries.erase(id);
			createNewMinimapOffscreenNotification = true;
		}
		// direction didn't change, but the precise location probably did
		else
		{
			auto spos = entry->sprite->getPosition();
			switch (currentDirection)
			{
			case 1:
			case 5:
				entry->sprite->setPosition(Vec2(pos.x, spos.y));
				break;
			case 3:
			case 7:
				entry->sprite->setPosition(Vec2(spos.x, pos.y));
				break;
			case 2:
			case 4:
			case 6:
			case 8:
				break;
			default:
				CCASSERT(0, "Invalid direction");
				break;
			}
		}
	}
	else if(currentDirection)
	{
		createNewMinimapOffscreenNotification = true;
	}

	if (createNewMinimapOffscreenNotification)
	{
		auto offscreenSprite = StaticHelpers::duplicateSprite(offscreenPrototype);
		offscreenSprite->setRotation(angle);
		if (offscreenSprite->getChildrenCount())
		{
			auto fg = offscreenSprite->getChildren().front();
			fg->setRotation(-1 * angle);
		}
		addChild(offscreenSprite);
		auto spriteSize = offscreenSprite->getBoundingBox().size;
		auto cornerVect = (screenSize - spriteSize) / 2;
		switch (currentDirection)
		{
		case 1:
			offscreenSprite->setPosition(Vec2(pos.x, screenSize.y / 2 - spriteSize.height));
			break;
		case 2:
			offscreenSprite->setPosition(cornerVect);
			break;
		case 3:
			offscreenSprite->setPosition(Vec2(screenSize.x / 2 - spriteSize.width, pos.y));
			break;
		case 4:
			offscreenSprite->setPosition(Vec2(cornerVect.x, -1 * cornerVect.y));
			break;
		case 5:
			offscreenSprite->setPosition(Vec2(pos.x, -1 * screenSize.y / 2 + spriteSize.height));
			break;
		case 6:
			offscreenSprite->setPosition(-1 * cornerVect);
			break;
		case 7:
			offscreenSprite->setPosition(Vec2(-1 * screenSize.x / 2 + spriteSize.width, pos.y));
			break;
		case 8:
			offscreenSprite->setPosition(Vec2(-1 * cornerVect.x, cornerVect.y));
			break;
		default:
			CCASSERT(0, "Invalid direction");
			break;
		}
		offscreenEntries[id] = new OffscreenElem(offscreenSprite, currentDirection);
	}
}

void Notifier::newOffscreenTorpedo(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenTorpedoPrototype, pos, id);
}

/// AI player
void Notifier::newOffscreenSubmarine(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenEnemyPrototype, pos, id);
}

void Notifier::newOffscreenCounterMeasure(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenCMPrototype, pos, id);
}

void Notifier::newOffscreenPing(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenPingPrototype, pos, id);
}

void Notifier::newOffscreenTubeFilling(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenTubeFillingPrototype, pos, id);
}

void Notifier::setLookingAt(Vec2 pos)
{
	lookingAt = pos;
}

/// 0: nothing removed
/// 1: only removed from the minimap
/// 2: removed from both the minimap and the offscreen notification
int Notifier::removeItem(int id)
{
	auto mme = minimapEntries.find(id);
	if (mme == minimapEntries.end())
		return 0;
	minimap->removeChild(mme->second->sprite, true);
	delete minimapEntries.find(id)->second;
	minimapEntries.erase(id);
	auto ose = offscreenEntries.find(id);
	// is detected but visible on screen
	if (ose == offscreenEntries.end())
		return 1;
	removeChild(ose->second->sprite, true);
	delete offscreenEntries.find(id)->second;
	offscreenEntries.erase(id);
	return 2;
}

void Notifier::setHPBar(float ratio)
{
	setBarPercentage("hp bar", ratio);
}

void Notifier::setThrustBar(float ratio)
{
	setBarPercentage("thrust bar", ratio);
}

void Notifier::setNoiseBar(float dB)
{
	auto ratio = dB < 0 ? 1 : dB / 100 * 2;
	setBarPercentage("noise bar", ratio);
}

void Notifier::setHPText(float hp)
{
	hpText->setString(std::to_string((int)std::round(hp)));
}

void Notifier::setThrustText(float thrust)
{
	auto xpos = thrust < 0 ? thrustNegativeX : thrustNormalX;
	thrustText->setPosition(Vec2(xpos, thrustText->getPosition().y));
	thrustText->setString(std::to_string((int)std::round(thrust)) + "%");
}

void Notifier::setNoiseText(float noise)
{
	std::string out = noise < 0 ? "deafening" : std::to_string((int)std::round(noise)) + " dB";
	noiseText->setString(out);
}

void Notifier::setSpeedText(float speed)
{
	speedText->setString(std::to_string((int)std::round(speed * OBJECT_SPEED_FUDGE)) + " kt");
}

void Notifier::setMenuText(std::string txt)
{
	menuText->setString(txt);
}

void Notifier::setActiveMenuText(std::string txt)
{
	activeMenuText->setString(txt);
}


void Notifier::update(float dt)
{
	timeSinceLastOpacityUpdate += dt;
	std::vector<int> toRemove;

	// minimap curation. Opacity modification / deleting old dots
	if (timeSinceLastOpacityUpdate >= MINIMAP_REDRAW_FREQ)
	{
		timeSinceLastOpacityUpdate -= MINIMAP_REDRAW_FREQ;
		for (auto kvp : minimapEntries)
		{
			auto elem = kvp.second;
			elem->ttl -= MINIMAP_REDRAW_FREQ;
			bool removeFlag = false;
			if (elem->ttl <= 0)
			{
				removeFlag = true;
			}
			else if(elem->isDotNode)
			{
				if(elem->dirty)
				{
					Vec2 swapTemp;
					for (auto i : elem->sprite->getChildren())
					{
						swapTemp = i->getPosition();
						i->setPosition(elem->nextPos);
						elem->nextPos = swapTemp;
					}
					elem->dirty = false;
				}
			}
			else if (elem->dirty)
			{
				elem->sprite->setPosition(elem->nextPos);
				elem->dirty = false;
			}

			if(removeFlag)
			{
				toRemove.push_back(kvp.first);
			}
			else if(elem->fading)
			{
				for (auto i : elem->sprite->getChildren())
				{
					if(i->isVisible())
					{
						i->setVisible(false);
						break;
					}
				}
				elem->dirty = true;
			}
		}
		// clean up since you can't erase collection elements while iterating over it
		for (int id : toRemove)
		{
			removeItem(id);
		}
	}

	for (auto kvp : minimapEntries)
	{
		kvp.second->sprite->update(dt);
		for (auto s : kvp.second->sprite->getChildren())
		{
			s->update(dt);
		}
	}
	for (auto kvp : offscreenEntries)
	{
		kvp.second->sprite->update(dt);
	}
}

int Notifier::isOffscreen(Vec2 pos)
{
	bool n, e, s, w;
	n = e = s = w = false;
	if(pos.y > lookingAt.y + screenSize.y / 2)
		n = true;
	else if(pos.y < lookingAt.y - screenSize.y / 2)
		s = true;
	if(pos.x > lookingAt.x + screenSize.x / 2)
		e = true;
	else if(pos.x < lookingAt.x - screenSize.x / 2)
		w = true;
	if(n && e)
		return 2;
	if(n && w)
		return 8;
	if(s && e)
		return 4;
	if(s && w)
		return 6;
	if(n)
		return 1;
	if(e)
		return 3;
	if(s)
		return 5;
	if(w)
		return 7;
	return 0;
}

/// ratio should have range [0, 1]
void Notifier::setBarPercentage(std::string name, float ratio)
{
	auto sprite = sprites.find(name)->second;
	float toShow = ratio * GUI_BAR_HEIGHT;
	sprite->setTextureRect(Rect(0, GUI_BAR_HEIGHT - toShow, 68, toShow));
}