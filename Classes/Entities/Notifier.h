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
/// Some sprites use cocos2d::Sprite instead of O3Sprite to use the StaticHelpers::duplicateSprite() function
/// This means no animated sprites for said nodes
class Notifier : public O3Sprite
{
public:
	Notifier(std::string resourceFolderPath, cocos2d::Vec2 screenSize);

	/// separate functions for each type for sprite allocation
	/// e.g. Ping has a different sprite compared to a torpedo
	/// pos is in game coordinates, not minimap coordinates
	void newMinimapEntry(O3Sprite* minimapSprite, 
		cocos2d::Vec2 pos, float ttl, bool isDrawNode,
		cocos2d::Color4F color = cocos2d::Color4F::MAGENTA);
	void newMinimapTorpedo(cocos2d::Vec2 pos);
	void newMinimapPlayer(cocos2d::Vec2 pos);
	void newMinimapSubmarine(cocos2d::Vec2 pos);
	void newMinimapCounterMeasure(cocos2d::Vec2 pos);
	void newMinimapPing(cocos2d::Vec2 pos);
	void newMinimapTubeFilling(cocos2d::Vec2 pos);

	void newOffscreenEntry(cocos2d::Sprite* offscreenPrototype, 
		cocos2d::Vec2 pos, float ttl, int id);
	void newOffscreenTorpedo(cocos2d::Vec2 pos, int id);
	void newOffscreenSubmarine(cocos2d::Vec2 pos, int id);
	void newOffscreenCounterMeasure(cocos2d::Vec2 pos, int id);
	void newOffscreenPing(cocos2d::Vec2 pos, int id);
	void newOffscreenTubeFilling(cocos2d::Vec2 pos, int id);

	/// could need to be independent of update()
	void setLookingAt(cocos2d::Vec2);

	void update(float dt);

private:
	struct MinimapElem
	{
		O3Sprite *sprite;
		float ttl;
		bool isDrawNode;
		cocos2d::Color4F color;

		MinimapElem(O3Sprite* s, float t, bool idn, cocos2d::Color4F c)
		{
			sprite = s;
			ttl = t;
			isDrawNode = idn;
			color = c;
		}
	};

	struct OffscreenElem
	{
		cocos2d::Sprite *sprite;
		int direction;
		float ttl;

		OffscreenElem(cocos2d::Sprite* s, float t, int dir)
		{
			sprite = s;
			ttl = t;
			direction = dir;
		}
	};

	/// is the position off screen? If so, which?
	/// 0: On screen
	/// 1: N
	/// 2: NE
	/// 3: E
	/// 4: SE
	/// 5: S
	/// 6: SW
	/// 7: W
	/// 8: NW
	int isOffscreen(cocos2d::Vec2 pos);

	std::string resourceFolderPath;
	/// key: Sprite ID
	std::unordered_map<int, MinimapElem*> minimapEntries;
	std::unordered_map<int, OffscreenElem*> offscreenEntries;
	float timeSinceLastOpacityUpdate;
	cocos2d::Vec2 screenSize, lookingAt;

	cocos2d::Sprite *minimap, *offscreenCMPrototype, *offscreenEnemyPrototype,
				*offscreenPingPrototype, *offscreenTorpedoPrototype,
				*offscreenTubeFillingPrototype;
};

#endif //__MINIMAP_H__