#include "Notifier.h"
#include "Helpers/Consts.h"
#include "Helpers/StaticHelpers.h"

#define SHOW_OFFSCREEN_OBJECTS

USING_NS_CC;
//TODO: replace DrawDot() with sprites and TintTo::

Notifier::Notifier(std::string path, Vec2 screenSize) : O3Sprite("1x1empty.png"),
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


	minimap = Sprite::create(resourceFolderPath + "/bg.png");
    Vec2 minimapPos = -1 * screenSize/2 + MINIMAP_SIZE/2;
    minimapPos.x += MINIMAP_SIZE.x/5;
    minimap->setPosition(minimapPos);
    addChild(minimap);
}

void Notifier::newMinimapEntry(O3Sprite* minimapSprite,
	Vec2 pos, float ttl, bool isDrawNode, Color4F color)
{
	#ifndef SHOW_OFFSCREEN_OBJECTS
	// necessary since culling should be done off screen instead of having
	// the torpedo disappear magically
	if(pos.x < -1*GAME_SIZE.x/2 || pos.x > GAME_SIZE.x/2 ||
		pos.y < -1*GAME_SIZE.y/2 || pos.y > GAME_SIZE.y/2)
	{
		// ignore objects outside of bounds
		return;
	}
	#endif
	minimapEntries[minimapSprite->getID()] = new MinimapElem(minimapSprite, ttl, isDrawNode, color);
	minimap->addChild(minimapSprite);
	float xrat = GAME_SIZE.x / MINIMAP_INTERNAL_SIZE.x;
	float yrat = GAME_SIZE.y / MINIMAP_INTERNAL_SIZE.y;
	// cocos2d::Node anchor is centered
	// cocos2d::Sprite anchor is bottom left
	minimapSprite->setPosition(Vec2(pos.x / xrat, pos.y / yrat) + MINIMAP_SIZE/2);
}

void Notifier::newMinimapTorpedo(Vec2 pos)
{
	auto minimapSprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 1, 1, 1);
	drawnode->drawDot(Vec2(0, 0), 1, color);
	minimapSprite->addChild(drawnode);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, color);
}

/// AI player
void Notifier::newMinimapSubmarine(Vec2 pos)
{
	auto minimapSprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 0, 0, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	minimapSprite->addChild(drawnode);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, color);
}

/// the human player
void Notifier::newMinimapPlayer(Vec2 pos)
{
	auto minimapSprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(0, 1, 0, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	minimapSprite->addChild(drawnode);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newMinimapCounterMeasure(Vec2 pos)
{
	auto minimapSprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(0, 0, 1, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	minimapSprite->addChild(drawnode);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newMinimapPing(Vec2 pos)
{
	auto minimapSprite = new O3Sprite("1x1empty.png");
	int count = 150;
	int fps = 30;
	minimapSprite->addAnimation("idle", resourceFolderPath + "/ping", count, fps);
	minimapSprite->setAnimation("idle");
	minimapSprite->playAnimation();
	newMinimapEntry(minimapSprite, pos, count / fps, false);
}

void Notifier::newMinimapTubeFilling(Vec2 pos)
{
	auto minimapSprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 0.5, 0, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	minimapSprite->addChild(drawnode);
	newMinimapEntry(minimapSprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newOffscreenEntry(Sprite* offscreenPrototype, 
	Vec2 pos, float ttl, int id)
{
	#ifndef SHOW_OFFSCREEN_OBJECTS
	// necessary since culling should be done off screen instead of having
	// the torpedo disappear magically
	if(pos.x < -1*GAME_SIZE.x/2 || pos.x > GAME_SIZE.x/2 ||
		pos.y < -1*GAME_SIZE.y/2 || pos.y > GAME_SIZE.y/2)
	{
		// ignore objects outside of bounds
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
		offscreenEntries[id] = new OffscreenElem(offscreenSprite, ttl, currentDirection);
	}
}

void Notifier::newOffscreenTorpedo(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenTorpedoPrototype, pos, MINIMAP_ICON_TTL, id);
}

/// AI player
void Notifier::newOffscreenSubmarine(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenEnemyPrototype, pos, MINIMAP_ICON_TTL, id);
}

void Notifier::newOffscreenCounterMeasure(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenCMPrototype, pos, MINIMAP_ICON_TTL, id);
}

void Notifier::newOffscreenPing(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenPingPrototype, pos, MINIMAP_ICON_TTL, id);
}

void Notifier::newOffscreenTubeFilling(Vec2 pos, int id)
{
	newOffscreenEntry(offscreenTubeFillingPrototype, pos, MINIMAP_ICON_TTL, id);
}

void Notifier::setLookingAt(Vec2 pos)
{
	lookingAt = pos;
}

void Notifier::update(float dt)
{
	timeSinceLastOpacityUpdate += dt;

	// minimap curation. Opacity modification / deleting old dots
	if (timeSinceLastOpacityUpdate >= MINIMAP_REDRAW_FREQ)
	{
		timeSinceLastOpacityUpdate -= MINIMAP_REDRAW_FREQ;
		std::vector<int> toRemove;
		for (auto kvp : minimapEntries)
		{
			auto elem = kvp.second;
			elem->ttl -= MINIMAP_REDRAW_FREQ;
			bool removeFlag = false;
			if (elem->ttl <= 0)
			{
				removeFlag = true;
			}
			else if(elem->isDrawNode)
			{
				elem->color.a = elem->color.a / MINIMAP_FADE_ACC;
				if(elem->color.a < MINIMAP_OPACITY_THRESHOLD)
				{
					removeFlag = true;
				}
				else
				{
					elem->sprite->removeAllChildren();
					DrawNode* drawnode = DrawNode::create();
					drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, elem->color);
					elem->sprite->addChild(drawnode);
				}
			}

			if(removeFlag)
			{
				minimap->removeChild(elem->sprite, true);
				toRemove.push_back(kvp.first);
			}
		}
		// clean up since you can't erase collection elements while iterating over it
		for (int id : toRemove)
		{
			delete minimapEntries.find(id)->second;
			minimapEntries.erase(id);
		}
	}

	std::vector<int> toRemove;
	for (auto kvp : offscreenEntries)
	{
		auto elem = kvp.second;
		elem->ttl -= MINIMAP_REDRAW_FREQ;
		if (elem->ttl <= 0)
		{
			removeChild(elem->sprite, true);
			toRemove.push_back(kvp.first);
		}
	}
	// clean up since you can't erase collection elements while iterating over it
	for (int id : toRemove)
	{
		delete offscreenEntries.find(id)->second;
		offscreenEntries.erase(id);
	}

	for (auto kvp : minimapEntries)
	{
		kvp.second->sprite->update(dt);
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