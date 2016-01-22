#include "Notifier.h"
#include "Helpers/Consts.h"

#define SHOW_OFFSCREEN_OBJECTS

USING_NS_CC;
//TODO: replace DrawDot() with sprites and TintTo::

Notifier::Notifier(std::string path, Vec2 screenSize) : O3Sprite("1x1empty.png"),
resourceFolderPath(path),
timeSinceLastOpacityUpdate(0),
screenSize(screenSize)
{
	minimap = new O3Sprite(resourceFolderPath + "/bg.png");
    Vec2 minimapPos = -1 * screenSize/2 + MINIMAP_SIZE/2;
    minimapPos.x += MINIMAP_SIZE.x/5;
    minimap->setPosition(minimapPos);
    addChild(minimap);
}

void Notifier::newEntry(O3Sprite* sprite, Vec2 pos, float ttl, bool isDrawNode, Color4F color)
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
	entries[sprite->getID()] = new NotifierElem(sprite, ttl, isDrawNode, color);
	minimap->addChild(sprite);
	float xrat = GAME_SIZE.x / MINIMAP_INTERNAL_SIZE.x;
	float yrat = GAME_SIZE.y / MINIMAP_INTERNAL_SIZE.y;
	sprite->setPosition(Vec2(pos.x / xrat, pos.y / yrat));
}

void Notifier::newTorpedo(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 1, 1, 1);
	drawnode->drawDot(Vec2(0, 0), 1, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newSubmarine(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 0, 0, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newPlayer(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(0, 1, 0, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newCounterMeasure(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(0, 0, 1, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::newPing(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	int count = 150;
	int fps = 30;
	sprite->addAnimation("idle", resourceFolderPath + "ping", count, fps);
	sprite->setAnimation("idle");
	sprite->playAnimation();
	newEntry(sprite, pos, count / fps, false);
}

void Notifier::newTubeFilling(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 0.5, 0, 1);
	drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Notifier::update(float dt)
{
	timeSinceLastOpacityUpdate += dt;

	// minimap curation. Opacity modification / deleting old dots
	if (timeSinceLastOpacityUpdate >= MINIMAP_REDRAW_FREQ)
	{
		timeSinceLastOpacityUpdate -= MINIMAP_REDRAW_FREQ;
		std::vector<int> toRemove;
		for (auto kvp : entries)
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
				minimap->removeChild(elem->sprite);
				toRemove.push_back(kvp.first);
			}
		}
		// clean up since you can't erase collection elements while iterating over it
		for (int id : toRemove)
		{
			delete entries.find(id)->second;
			entries.erase(id);
		}
	}

	for (auto kvp : entries)
	{
		kvp.second->sprite->update(dt);
	}
}