#ifndef __CONSTS_H__
#define __CONSTS_H__

#include "cocos2d.h"

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

static const cocos2d::Vec2 GRID_SPACING = cocos2d::Vec2(20, 20);
static const cocos2d::Vec2 GRID_LABEL_SPACING = GRID_SPACING * 5;
static const cocos2d::Vec2 CURSOR_OFFSET = cocos2d::Vec2(12, -14);

static const std::string SOUND_PATH = "Sounds/";

enum class PhysicsModel
{
    Stationary,
    SpeedOnly,
    Newtonian
};

#endif // __CONSTS_H__
