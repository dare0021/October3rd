#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "cocos2d.h"
#include "O3Sprite.h"
#include <unordered_map>

/// update functions can take entries about formerly not mentioned items
/// just run a function for each item in the game as necessary
/// e.g. anything the player shouldn't be able to see shouldn't be on the minimap
/// designed to tack on a new transient item (sprite fades with time) each interval
/// interval should be independent of the update rate
/// each sprite goes from 255 opacity to 0 within their time to live with decreasing delta
class Notifier : public O3Sprite
{
public:
	Notifier(std::string resourceFolderPath, cocos2d::Vec2 screenSize);

	/// separate functions for each type for sprite allocation
	/// e.g. Ping has a different sprite compared to a torpedo
	/// pos is in game coordinates, not minimap coordinates
	void newEntry(O3Sprite*, cocos2d::Vec2 pos, float ttl, bool isDrawNode, cocos2d::Color4F color = cocos2d::Color4F::MAGENTA);
	void newTorpedo(cocos2d::Vec2 pos);
	void newPlayer(cocos2d::Vec2 pos);
	void newSubmarine(cocos2d::Vec2 pos);
	void newCounterMeasure(cocos2d::Vec2 pos);
	void newPing(cocos2d::Vec2 pos);
	void newTubeFilling(cocos2d::Vec2 pos);

	void update(float dt);

private:
	struct NotifierElem
	{
		O3Sprite* sprite;
		float ttl;
		bool isDrawNode;
		cocos2d::Color4F color;

		NotifierElem(O3Sprite* s, float t, bool idn, cocos2d::Color4F c)
		{
			sprite = s;
			ttl = t;
			isDrawNode = idn;
			color = c;
		}
	};

	std::string resourceFolderPath;
	/// key: Sprite ID
	std::unordered_map<int, NotifierElem*> entries;
	float timeSinceLastOpacityUpdate;
	cocos2d::Vec2 screenSize;

	O3Sprite* minimap;
};

#endif //__MINIMAP_H__