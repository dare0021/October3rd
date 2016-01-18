#ifndef __CONSTS_H__
#define __CONSTS_H__

#include "cocos2d.h"

static const int UI_DEPTH = 1000;
static const int CURSOR_DEPTH = 9999;

static const float TARGET_FPS = 60;
static const float TYPE_TIME_MAX = 0.25f;
static const float ACC_G = 9.80665f;
static const float ZERO_SPEED_THRESHOLD = 1.0f;
static const float COMMOROSE_MIN_DIST = 50.0f;
static const float PROTRACTOR_SIZE = 200;
static const float PROTRACTOR_LABEL_SIZE = 15;
static const float PROTRACTOR_LINE_THICKNESS = 0.5;
static const float GRID_LINE_THICKNESS = 0.5;
static const float GRID_LABEL_SIZE = 15;
static const float DOUBLECLICK_THRESHOLD = 0.3;
/// how often the minimap items are redrawn in ms
static const float MINIMAP_REDRAW_TICK = 100;
static const float MINIMAP_FADE_ACC = 1.5;
/// how long a minimap item remains on the screen including fade out in ms
static const float MINIMAP_ICON_TTL = 1000;

static const cocos2d::Vec2 GRID_SPACING = cocos2d::Vec2(20, 20);
static const cocos2d::Vec2 GRID_LABEL_SPACING = GRID_SPACING * 5;
static const cocos2d::Vec2 CURSOR_OFFSET = cocos2d::Vec2(12, -14);
static const cocos2d::Vec2 MINIMAP_SIZE = cocos2d::Vec2(304, 304);
static const cocos2d::Vec2 MINIMAP_INTERNAL_SIZE = cocos2d::Vec2(284, 284);
static const cocos2d::Vec2 GAME_SIZE = cocos2d::Vec2(10000, 10000);

static const std::string SOUND_PATH = "sounds/";
static const std::string DATA_PATH = "data/";
static const std::string PLAYER_SUB_NAME = "player_sub";

enum class PhysicsModel
{
    Stationary,
    SpeedOnly,
    Newtonian
};

#endif // __CONSTS_H__
