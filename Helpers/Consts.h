#ifndef __CONSTS_H__
#define __CONSTS_H__

#include "cocos2d.h"

static const int UI_DEPTH = 1000;
static const int CURSOR_DEPTH = 9999;
/// number of icons used per entry;
static const int MINIMAP_DOT_ANIM_COUNT = 20;
static const int MINIMAP_DOT_FPS = 5;

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
/// how often the minimap items are redrawn in seconds
static const float MINIMAP_REDRAW_FREQ = 0.1;
/// how long the offscreen notification remains on the screen during fade out in seconds
static const float MINIMAP_ICON_TTL = 5;
/// how often the game culls out of bounds objects, excluding submarines in seconds
static const float OBJECT_CULL_FREQ = 1;

static const cocos2d::Vec2 GRID_SPACING = cocos2d::Vec2(20, 20);
static const cocos2d::Vec2 GRID_LABEL_SPACING = GRID_SPACING * 5;
static const cocos2d::Vec2 CURSOR_OFFSET = cocos2d::Vec2(12, -14);
static const cocos2d::Vec2 MINIMAP_SIZE = cocos2d::Vec2(304, 304);
static const cocos2d::Vec2 MINIMAP_INTERNAL_SIZE = cocos2d::Vec2(284, 284);
static const cocos2d::Vec2 GAME_SIZE = cocos2d::Vec2(10000, 10000);

static const std::string SOUND_PATH = "sounds/";
static const std::string DATA_PATH = "data/";
static const std::string PLAYER_SUB_NAME = "player_sub";
/// the blip icon
static const std::string MINIMAP_ICON_PATH = "dots/";

enum class PhysicsModel
{
    Stationary,
    SpeedOnly,
    Newtonian
};

#endif // __CONSTS_H__
