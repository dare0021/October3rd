#ifndef __ANIMDATA_H__
#define __ANIMDATA_H__

#include "cocos2d.h"
#include "Helpers/Consts.h"
#include <unordered_map>

/// Not to be used as is. Use as a child of O3Sprite or its inheritor nodes
class AnimData
{
public:
	struct AnimElem
	{
		std::string name;
		float lifetime;

		AnimElem(std::string n, float lt)
		{
			name = n;
			lifetime = lt;
		}
	};

	AnimData(std::string path, int count, float frameRate = TARGET_FPS, bool loop = true);
	AnimData(std::vector<AnimElem*> preprocessedData, bool loop = true);
	~AnimData();

	void update(float dt);

	std::string getLastAccessedName();
	std::string getCurrentName();
	int getFrameNumber();
	bool isLoop();
	bool isDone();

private:
	std::vector<AnimElem*> data;
	/// an iterator over the above variable called data
	std::vector<AnimElem*>::iterator currentElem;
	/// Leftover time on last update
	float timeOffset;
	bool _isLoop, _isDone;
	std::string lastAccessedName;
};

#endif // __ANIMDATA_H__
