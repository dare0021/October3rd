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
		cocos2d::Vec2 pos, float ttl, bool isDotNode, int id,
		std::string dotPath);
	void newMinimapTorpedo(cocos2d::Vec2 pos, int id);
	void newMinimapPlayer(cocos2d::Vec2 pos, int id);
	void newMinimapSubmarine(cocos2d::Vec2 pos, int id);
	void newMinimapCounterMeasure(cocos2d::Vec2 pos, int id);
	void newMinimapPing(cocos2d::Vec2 pos, int id);
	void newMinimapTubeFilling(cocos2d::Vec2 pos, int id);

	void newOffscreenEntry(cocos2d::Sprite* offscreenPrototype, 
		cocos2d::Vec2 pos, int id);
	void newOffscreenTorpedo(cocos2d::Vec2 pos, int id);
	void newOffscreenSubmarine(cocos2d::Vec2 pos, int id);
	void newOffscreenCounterMeasure(cocos2d::Vec2 pos, int id);
	void newOffscreenPing(cocos2d::Vec2 pos, int id);
	void newOffscreenTubeFilling(cocos2d::Vec2 pos, int id);

	/// could need to be independent of update()
	void setLookingAt(cocos2d::Vec2);
	int removeItem(int id);

	void update(float dt);

private:
	struct MinimapElem
	{
		O3Sprite *sprite;
		float ttl;
		bool isDotNode, dirty, fading;
		cocos2d::Vec2 nextPos;

		MinimapElem(O3Sprite* s, float t, bool idn)
		{
			sprite = s;
			ttl = t;
			isDotNode = idn;
			fading = dirty = false;
			nextPos = cocos2d::Vec2::ZERO;
		}
	};

	struct OffscreenElem
	{
		cocos2d::Sprite *sprite;
		int direction;
		bool fading;

		OffscreenElem(cocos2d::Sprite* s, int dir)
		{
			sprite = s;
			direction = dir;
			fading = false;
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
	/// key: Sprite ID of the object represented
	std::unordered_map<int, MinimapElem*> minimapEntries;
	std::unordered_map<int, OffscreenElem*> offscreenEntries;
	float timeSinceLastOpacityUpdate;
	cocos2d::Vec2 screenSize, lookingAt;

	cocos2d::Sprite *minimap, *offscreenCMPrototype, *offscreenEnemyPrototype,
				*offscreenPingPrototype, *offscreenTorpedoPrototype,
				*offscreenTubeFillingPrototype;
};

#endif //__MINIMAP_H__