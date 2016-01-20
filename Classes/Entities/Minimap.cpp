#include "Minimap.h"
#include "Helpers/Consts.h"

USING_NS_CC;
//TODO: replace DrawDot() with sprites and TintTo::

Minimap::Minimap(std::string path) : O3Sprite(path + "/bg.png"),
resourceFolderPath(path),
timeSinceLastOpacityUpdate(0)
{

}

void Minimap::newEntry(O3Sprite* sprite, Vec2 pos, float ttl, bool isDrawNode, Color4F color)
{
	entries[sprite->getID()] = new MinimapElem(sprite, ttl, isDrawNode, color);
	addChild(sprite);
	float xrat = GAME_SIZE.x/2/MINIMAP_INTERNAL_SIZE.x;
	float yrat = GAME_SIZE.y/2/MINIMAP_INTERNAL_SIZE.y;
	sprite->setPosition(Vec2(pos.x/xrat, pos.y/yrat));
}

void Minimap::newTorpedo(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 1, 1, 1);
	drawnode->drawDot(Vec2(0,0), 1, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Minimap::newSubmarine(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 0, 0, 1);
	drawnode->drawDot(Vec2(0,0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Minimap::newPlayer(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(0, 1, 0, 1);
	drawnode->drawDot(Vec2(0,0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Minimap::newCounterMeasure(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(0, 0, 1, 1);
	drawnode->drawDot(Vec2(0,0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Minimap::newPing(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	int count = 150;
	int fps = 30;
	sprite->addAnimation("idle", resourceFolderPath + "ping", count, fps);
	sprite->setAnimation("idle");
	sprite->playAnimation();
	newEntry(sprite, pos, count/fps, false);
}

void Minimap::newTubeFilling(Vec2 pos)
{
	auto sprite = new O3Sprite("1x1empty.png");
	auto drawnode = DrawNode::create();
	Color4F color = Color4F(1, 0.5, 0, 1);
	drawnode->drawDot(Vec2(0,0), MINIMAP_ICON_SIZE, color);
	sprite->addChild(drawnode);
	newEntry(sprite, pos, MINIMAP_ICON_TTL, true, color);
}

void Minimap::update(float dt)
{
	timeSinceLastOpacityUpdate += dt;

	if(timeSinceLastOpacityUpdate >= MINIMAP_REDRAW_TICK)
	{
		timeSinceLastOpacityUpdate -= MINIMAP_REDRAW_TICK;
		std::vector<int> toRemove;
		for (auto kvp : entries)
		{
			auto elem = kvp.second;
			elem->ttl -= MINIMAP_REDRAW_TICK;
			if(elem->ttl <= 0)
			{
				removeChild(elem->sprite);
				toRemove.push_back(kvp.first);
			}
			else
			{
				elem->color.a = elem->color.a / MINIMAP_FADE_ACC;
				DrawNode* drawnode = (DrawNode*)elem->sprite->getChildren().front();
				drawnode->clear();
				drawnode->setOpacity((int)(255 * elem->color.a)); //no worky
				drawnode->drawDot(Vec2(0, 0), MINIMAP_ICON_SIZE, elem->color);
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
	CCLOG("Num entries %d", entries.size());
}