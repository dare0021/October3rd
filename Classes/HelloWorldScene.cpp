#include "HelloWorldScene.h"

#include "ui/CocosGUI.h"

#include "Helpers/StaticHelpers.h"
#include "Helpers/Consts.h"
#include "Entities/Commorose.h"
#include "Entities/Protractor.h"
#include "Entities/Notifier.h"

// when not locked, there is no guarantee the camera will always point to the
// player sub. e.g. when player sub location is modified by bounds checker
//#define LOCK_PLAYER_CAMERA

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    timeSinceLastMouseUp = 9999;
    timeSinceLastNotifierUpdate = 0;
    timeSinceLastObjectCull = 0;

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.x/2 + origin.x, visibleSize.y/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    overlaySprite = Sprite::create();
    overlaySprite->setPosition(visibleSize/2);
    this->addChild(overlaySprite, UI_DEPTH);

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    // position the label on the center of the screen
    label->setPosition(Vec2(0,label->getContentSize().height/2));
    // add the label as a child to this layer
    overlaySprite->addChild(label, 1);

    cursorSprite = Sprite::create("cursors/arrow.png");
    addChild(cursorSprite, CURSOR_DEPTH);
    cursorSprite->setScale(0.7);

    protractor = (Sprite*) new Protractor("protractor", PROTRACTOR_SIZE);
    addChild(protractor, 1);

	commorose = (Sprite*) new Commorose("commorose");
    addChild(commorose, CURSOR_DEPTH - 1);
    commorose->setVisible(false);

    TorpedoData::init("torpedos.jld");

    playerSub = (Sprite*) new Submarine("player-sub.png");
    auto s = (Submarine*)playerSub;
    s->setName(PLAYER_SUB_NAME);
    s->setPhysicsModel(PhysicsModel::Newtonian);
    s->setMass(300);
    s->setFriction(5);
    s->setMaxForce(20 * 1000);
    s->setPropulsionType(PropulsionType::Diesel);
    s->setNoisiness(40);
    addChild(playerSub, UI_DEPTH);
    playerDeltaForcePerSecond = 2000;
    playerTurning = false;

    ((Submarine*)playerSub)->addTorpedoPrototype(new TorpedoData("testpedo", "testpedo"));

    notifier = (Sprite*) new Notifier("notifier", visibleSize);
    ((Notifier*)notifier)->setThrustBar(0);
    ((Notifier*)notifier)->setNoiseBar(0);
    addChild(notifier, UI_DEPTH);

    moveScreenBy(Director::getInstance()->getVisibleSize()/-2);
    lookAt(Vec2::ZERO);
    this->scheduleUpdate();
    return true;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();

    auto dispatcher = Director::getInstance()->getEventDispatcher();

    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    keyListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    mouseListener->onMouseScroll = CC_CALLBACK_1(HelloWorld::onMouseScroll, this);
    dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

#ifdef MOUSE_DOUBLE_LISTEN_FUDGE
    mouseDownFudge = false;
    mouseUpFudge = false;
    mouseMoveFudge = false;
    mouseScrollFudge = false;
#endif
    isMouseDown[0] = false;
    isMouseDown[1] = false;
    isMouseDown[2] = false;
    typeKeyCandidates.clear();
}

void HelloWorld::onExit()
{
    Layer::onExit();
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    lastKey = keyCode;
    activeKeys.push_back(keyCode);
    typeKeyCandidates.insert({keyCode, new float(0)});
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    int toRemove = 0;
    for (auto k : activeKeys)
    {
        if(k == keyCode)
            break;
        toRemove++;
    }
    if(toRemove < (int)activeKeys.size())
    {
        activeKeys.erase(activeKeys.begin()+toRemove);
    }

    if(*typeKeyCandidates.find(keyCode)->second <= TYPE_TIME_MAX)
        onKeyTyped(keyCode);
    typeKeyCandidates.erase(keyCode);
}

///Pseudo-listener
void HelloWorld::onKeyTyped(EventKeyboard::KeyCode keyCode)
{
    switch (keyCode)
    {
		case EventKeyboard::KeyCode::KEY_SPACE:
		{
            auto c = (Commorose*)commorose;
            auto s = (Submarine*)playerSub;
            if(c->getMode() == 3) //weapons
            {
                addTorpedo(s->spawnTorpedo("testpedo", ((Protractor*)protractor)->getCursorAngle()));
            }
            break;
		}
    }
}

void HelloWorld::onMouseDown(Event* event)
{
#ifdef MOUSE_DOUBLE_LISTEN_FUDGE
    mouseDownFudge = !mouseDownFudge;
    if(!mouseDownFudge)
        return;
#endif
    EventMouse* e = (EventMouse*)event;
    isMouseDown[e->getMouseButton()] = true;
    std::stringstream ss;
    ss << "Mouse Down ";
    ss << e->getMouseButton();

    if(e->getMouseButton() == 1)
    {
        if(!getChildByName("animtest"))
        {
            auto s = new O3Sprite("", true);
            addSprite(s);
            s->addAnimation("idle", "animtest", 9, 12);
            s->setAnimation("idle");
            s->playAnimation();
            s->setName("animtest");
        }
        else
        {
            removeChildByName("animtest");
        }
    }
}

void HelloWorld::onMouseUp(Event* event)
{
#ifdef MOUSE_DOUBLE_LISTEN_FUDGE
    mouseUpFudge = !mouseUpFudge;
    if(!mouseUpFudge)
        return;
#endif
    EventMouse* e = (EventMouse*)event;
    isMouseDown[e->getMouseButton()] = false;
    timeSinceLastMouseUp = 0;
    std::stringstream ss;
    ss << "Mouse Up detected, Key: ";
    ss << e->getMouseButton();
}

///Referencing previous location results in garbage values
///Use only for hover
void HelloWorld::onMouseMove(Event* event)
{
#ifdef MOUSE_DOUBLE_LISTEN_FUDGE
    mouseMoveFudge = !mouseMoveFudge;
    if(!mouseMoveFudge)
        return;
#endif
    EventMouse* e = (EventMouse*)event;
    lastCursor.x = e->getCursorX();
    lastCursor.y = e->getCursorY();
    repaintCursor();
    Protractor* p = (Protractor*)protractor;
    p->setCursorAngle(StaticHelpers::headingAngle(lastCursor - Director::getInstance()->getVisibleSize()/2));

    if(isMouseDown[0])
    {//LMB drag

    }
    if(isMouseDown[1])
    {//RMB drag

    }
    if(isMouseDown[2])
    {//MMB drag

    }

    std::stringstream ss;
    ss << "MousePosition X:";
    ss << e->getCursorX() << " Y:" << e->getCursorY();
}

///Input is 1 or -1
void HelloWorld::onMouseScroll(Event* event)
{
#ifdef MOUSE_DOUBLE_LISTEN_FUDGE
    mouseScrollFudge = !mouseScrollFudge;
    if(!mouseScrollFudge)
        return;
#endif
    EventMouse* e = (EventMouse*)event;
    if(e->getScrollY() > 0)
    {
        auto vect = screenspaceToWorldspace(lastCursor);
    }
    else if(e->getScrollY() < 0)
    {
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* e)
{
    return true;
}

///Only works for LMB drag, but at least it works
void HelloWorld::onTouchMoved(Touch* touch, Event* e)
{
    bool rmb = isMouseDown[1];
    bool mmb = isMouseDown[2];
    Vec2 fullDelta = touch->getLocation() - touch->getStartLocation();
    float headingAngle = StaticHelpers::headingAngle(fullDelta);
    if(rmb)
    {//RMB drag
        CCLOG("RMB");
        moveScreenBy(-1*touch->getDelta());
    }
    if(!(rmb || mmb))
    {
        Commorose* c = (Commorose*)commorose;
        if(!c->isVisible())
        {
            c->setPositionOnScreen(touch->getLocation());
            c->setVisible(true);
        }
        else
        {
            if(fullDelta.length() > COMMOROSE_MIN_DIST)
                c->setCursorAngle(headingAngle);   
            else
                c->setCursorAngle(-90);
        }
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* e)
{
    Commorose* c = (Commorose*)commorose;
    if(c->isVisible())
    {
        removeChild(cursorSprite);
        switch (c->getMode()) //TODO: do something with this
        {
            case -1:
            {
                cursorSprite = Sprite::create("cursors/arrow.png");
                break;
            }
            case 0:
            {
                cursorSprite = Sprite::create("cursors/speed.png");
                break;
            }
            case 1:
            {
                cursorSprite = Sprite::create("cursors/defences.png");
                break;
            }
            case 2:
            {
                cursorSprite = Sprite::create("cursors/sensors.png");
                break;
            }
            case 3:
            {
                cursorSprite = Sprite::create("cursors/weapons.png");
                break;
            }
        }
        addChild(cursorSprite, CURSOR_DEPTH);
        repaintCursor();
        commorose->setVisible(false);
        return;
    }
    if(timeSinceLastMouseUp < DOUBLECLICK_THRESHOLD)
    {
        auto p = (Protractor*)protractor;
        ((O3Sprite*)playerSub)->setTargetHeading(p->getCursorAngle());
    }
}

void HelloWorld::repaintCursor()
{
    cursorSprite->setPosition(pointingAt() + CURSOR_OFFSET);
}

///all screen move functions should use this
void HelloWorld::lookAt(Vec2 pos)
{
    Vec2 visible = Director::getInstance()->getVisibleSize();
    this->setPosition(-1*(pos - visible/2));
    protractor->setPosition(lookingAt());
    notifier->setPosition(lookingAt());

    //draw grid
    Vec2 botleft = screenspaceToWorldspace(Vec2::ZERO);
    Vec2 topright = screenspaceToWorldspace(visible);
    Vec2 drawSpaceBotLeft = botleft - GRID_LABEL_SPACING;
    Vec2 drawSpaceTopRight = topright + GRID_LABEL_SPACING;
	DrawNode* gridSprite = (DrawNode*)getChildByName("gridSprite");
    if (gridSprite)
    {
        gridSprite->clear();
        std::vector<std::string> toRemove;
        for (auto child : gridSprite->getChildren())
        {
            if (!onScreen(child->getPosition(), Vec2(GRID_LABEL_SPACING)))
                toRemove.push_back(child->getName());
        }
        for (auto name : toRemove)
        {
            gridSprite->removeChildByName(name);
        }
    }
    else
    {
        gridSprite = DrawNode::create();
        gridSprite->setName("gridSprite");
        addChild(gridSprite, 0);
    }
    //vertical lines
    for (int i= drawSpaceBotLeft.x; i<drawSpaceTopRight.x; i++)
    {
        if(i % (int)GRID_SPACING.x)
            continue;
        gridSprite->drawSegment(Vec2(i, drawSpaceBotLeft.y), Vec2(i, drawSpaceBotLeft.y+visible.y+2*GRID_LABEL_SPACING.y),
                                GRID_LINE_THICKNESS, Color4F(1,1,1,0.1));
        if(!(i % (int)GRID_LABEL_SPACING.x))
        {
            std::string txt = std::to_string(i);
            auto label = gridSprite->getChildByName("v" + txt);
            if (!label)
            {
                label = Label::createWithTTF(txt, "fonts/arial.ttf", GRID_LABEL_SIZE);
                label->setName("v" + txt);
                gridSprite->addChild(label);
            }
            label->setPosition(Vec2(i, topright.y - GRID_LABEL_SIZE * 2));
        }
    }
    //horizontal lines
    for (int i= drawSpaceBotLeft.y; i<drawSpaceTopRight.y; i++)
    {
        if(i % (int)GRID_SPACING.y)
            continue;
        gridSprite->drawSegment(Vec2(drawSpaceBotLeft.x, i), Vec2(drawSpaceBotLeft.x+visible.x + 2 * GRID_LABEL_SPACING.x, i),
                                GRID_LINE_THICKNESS, Color4F(1,1,1,0.1));   
        if(!(i % (int)GRID_LABEL_SPACING.y))
        {
            std::string txt = std::to_string(i);
            auto label = gridSprite->getChildByName("h" + txt);
            if (!label)
            {
                label = Label::createWithTTF(txt, "fonts/arial.ttf", GRID_LABEL_SIZE);
                label->setName("h" + txt);
                gridSprite->addChild(label);
            }
            label->setPosition(Vec2(botleft.x + GRID_LABEL_SIZE * 2, i));
        }
    }
    repaintCursor();
    Commorose* c = (Commorose*)commorose;
    c->setPosition(screenspaceToWorldspace(c->getPositionOnScreen()));
}

Vec2 HelloWorld::lookingAt()
{
    return -1 * this->getPosition() + Director::getInstance()->getVisibleSize()/2;
}

Vec2 HelloWorld::pointingAt()
{
    return -1 * this->getPosition() + lastCursor;
}

void HelloWorld::moveScreenBy(Vec2 diff)
{
    lookAt(-1 * this->getPosition() + Director::getInstance()->getVisibleSize()/2 + diff);
}

bool HelloWorld::onScreen(Vec2 pos)
{
    return onScreen(pos, Vec4::ZERO);
}

bool HelloWorld::onScreen(Vec2 pos, Vec2 margin)
{
    return onScreen(pos, Vec4(margin.x, margin.y, margin.x, margin.y));
}

/// Checks the vector itself
/// Be careful for edge cases where the anchor is off screen but the object itself is not
/// margin is top right bottom left
bool HelloWorld::onScreen(Vec2 pos, Vec4 margin)
{
    Vec2 visible = Director::getInstance()->getVisibleSize();
    auto sspos = worldspaceToScreenspace(pos);
    if(sspos.x < -1*margin.w || sspos.y < -1*margin.z)
        return false;
    if(sspos.x > visible.x+margin.x || sspos.y > visible.y+margin.y)
        return false;
    return true;
}

Vec2 HelloWorld::screenspaceToWorldspace(Vec2 sspos)
{
    Vec2 origin = -1 * this->getPosition();
    return sspos + origin;
}

Vec2 HelloWorld::worldspaceToScreenspace(Vec2 wspos)
{
    Vec2 origin = -1 * this->getPosition();
    return wspos - origin;
}

void HelloWorld::update(float dt)
{
    auto n = (Notifier*) notifier;
    timeSinceLastMouseUp += dt;
    timeSinceLastNotifierUpdate += dt;
    timeSinceLastObjectCull += dt;
    Vec2 visibleSize = Director::getInstance()->getVisibleSize();
    bool updateMinimap = timeSinceLastNotifierUpdate >= MINIMAP_REDRAW_FREQ;

    // cull first, then update the remainder
    if(timeSinceLastObjectCull >= OBJECT_CULL_FREQ)
    {
        timeSinceLastObjectCull -= OBJECT_CULL_FREQ;
        std::vector<int> toRemove;
        for (auto sprite : torpedoVect)
        {
            auto bounds = (GAME_SIZE + visibleSize) / 2;
            if(sprite->getPosition().x < -1* bounds.x ||
                sprite->getPosition().x > bounds.x ||
                sprite->getPosition().y < -1*bounds.y ||
                sprite->getPosition().y > bounds.y)
            {
                toRemove.push_back(sprite->getID());
            }
        }
        for (int i : toRemove)
        {
            removeTorpedoByID(i);
        }

        toRemove.clear();
        for (auto sprite : spriteVect)
        {
            auto bounds = (GAME_SIZE + visibleSize) / 2;
            if(sprite->getPosition().x < -1* bounds.x ||
                sprite->getPosition().x > bounds.x ||
                sprite->getPosition().y < -1*bounds.y ||
                sprite->getPosition().y > bounds.y)
            {
                toRemove.push_back(sprite->getID());
            }
        }
        for (int i : toRemove)
        {
            removeSpriteByID(i);
        }
    }

    // redirect the player sub if it veers off map
    //TODO: enemy subs
    if(playerSub->getPosition().x < -1*GAME_SIZE.x/2)
        playerSub->setPosition(-1*GAME_SIZE.x/2, playerSub->getPosition().y);
    else if(playerSub->getPosition().x > GAME_SIZE.x/2)
        playerSub->setPosition(GAME_SIZE.x/2, playerSub->getPosition().y);
    if(playerSub->getPosition().y < -1*GAME_SIZE.y/2)
        playerSub->setPosition(playerSub->getPosition().x, -1*GAME_SIZE.y/2);
    else if(playerSub->getPosition().y > GAME_SIZE.y/2)
        playerSub->setPosition(playerSub->getPosition().x, GAME_SIZE.y/2);

    if(updateMinimap)
        timeSinceLastNotifierUpdate -= MINIMAP_REDRAW_FREQ;

    n->setLookingAt(lookingAt());

    for (auto p : typeKeyCandidates)
    {
        *p.second += dt;
    }

    // process user input before updating existing items
    // while key pressed
    for (auto key : activeKeys)
    {
        auto ps = (Submarine*)playerSub;
		switch (key)
		{
			case EventKeyboard::KeyCode::KEY_UP_ARROW:
			{
				setPlayerForce(getPlayerForce() + playerDeltaForcePerSecond * dt);
				break;
			}
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			{
				setPlayerForce(getPlayerForce() - playerDeltaForcePerSecond * dt);
				break;
			}
#ifdef TURN_VIA_ARROW_KEYS
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            {
                ps->setTargetHeading(ps->getTargetHeading() + ps->getTurnSpeed() * dt);
                break;
            }
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            {
                ps->setTargetHeading(ps->getTargetHeading() - ps->getTurnSpeed() * dt);
                break;
            }
#endif
		}
    }

    // update existing items
    n->update(dt);
    for (auto o3s : spriteVect)
    {
        o3s->update(dt);
    }
    for (auto torpedo : torpedoVect)
    {
        torpedo->update(dt);
        if(updateMinimap)
            n->newMinimapTorpedo(torpedo->getPosition(), torpedo->getID());
		n->newOffscreenTorpedo(torpedo->getPosition(), torpedo->getID());
    }

    lastPlayerPos = playerSub->getPosition();
    if(playerTurning && !((Submarine*)playerSub)->isTurning())
    {
        playerTurning = false;
        setPlayerForce(getPlayerForce());
    }
    else if(!playerTurning && ((Submarine*)playerSub)->isTurning())
    {
        playerTurning = true;
        setPlayerForce(getPlayerForce());
    }
    playerSub->update(dt);
    // update speed display after physics simulation
    ((Notifier*)notifier)->setSpeedText(((Submarine*)playerSub)->getSpeed());

    if(updateMinimap)
        n->newMinimapPlayer(playerSub->getPosition(), ((O3Sprite*)playerSub)->getID());
    #ifdef LOCK_PLAYER_CAMERA
    lookAt(playerSub->getPosition());
    #else
    moveScreenBy(playerSub->getPosition() - lastPlayerPos);
    #endif
}

O3Sprite* HelloWorld::getSpriteByName(std::string name)
{
    for (auto s : spriteVect)
    {
        if(s->getName() == name)
            return s;
    }
    return nullptr;
}

void HelloWorld::addSprite(O3Sprite* sprite, bool addToSpriteVect)
{
    if(addToSpriteVect)
        spriteVect.push_back(sprite);
    this->addChild(sprite);
}

int HelloWorld::removeSpriteByName(std::string name)
{
    std::list<int> toRemove;
    int i = 0;
    for (auto s : spriteVect)
    {
        if(s->getName() == name)
            toRemove.push_front(i);
        i++;
    }
    for (auto j : toRemove)
    {
        this->removeChild(*(spriteVect.begin() + j));
        spriteVect.erase(spriteVect.begin() + j);
    }
    return toRemove.size();
}

bool HelloWorld::removeSpriteByID(int id)
{
    int i = 0;
    bool foundOne = false;
    for (auto s : spriteVect)
    {
        if(s->getID() == id)
        {
            foundOne = true;
            break;
        }
        i++;
    }
    if(!foundOne)
        return false;
    this->removeChild(*(spriteVect.begin() + i));
    spriteVect.erase(spriteVect.begin() + i);
    return true;
}

void HelloWorld::addTorpedo(Torpedo* torpedo)
{
    torpedoVect.push_back(torpedo);
    addSprite(torpedo, false);
}

bool HelloWorld::removeTorpedoByID(int id)
{
    int i = 0;
    bool foundOne = false;
    for (auto t : torpedoVect)
    {
        if(t->getID() == id)
        {
            foundOne = true;
            break;
        }
        i++;
    }
    if(!foundOne)
        return false;
    this->removeChild(*(torpedoVect.begin() + i));
    torpedoVect.erase(torpedoVect.begin() + i);
    return true;
}

void HelloWorld::setPlayerHP(float nhp)
{
    ((Submarine*)playerSub)->setHP(nhp);
    ((Notifier*)notifier)->setHPBar(getPlayerHP() / getPlayerMaxHP());
    ((Notifier*)notifier)->setHPText(getPlayerHP());
}

float HelloWorld::getPlayerHP()
{
    return ((Submarine*)playerSub)->getHP();
}

void HelloWorld::setPlayerMaxHP(float nhp)
{
    ((Submarine*)playerSub)->setMaxHP(nhp);
}

float HelloWorld::getPlayerMaxHP()
{
    return ((Submarine*)playerSub)->getMaxHP();
}

void HelloWorld::setPlayerForce(float nhp)
{
    auto s = (Submarine*)playerSub;
    auto n = (Notifier*)notifier;
    s->setForce(nhp);
    float displayForce = getPlayerForce();
    if(playerTurning)
        displayForce += s->getTurningForce();
	float displayRatio = displayForce / getPlayerMaxForce();
    n->setThrustBar(displayRatio);
    n->setThrustText(displayRatio * 100);
    float noise = s->getNoiseLevel(displayRatio);
    n->setNoiseBar(noise);
    n->setNoiseText(noise);
}

float HelloWorld::getPlayerForce()
{
    return ((Submarine*)playerSub)->getForce();
}

void HelloWorld::setPlayerMaxForce(float nhp)
{
    ((Submarine*)playerSub)->setMaxForce(nhp);
}

float HelloWorld::getPlayerMaxForce()
{
    return ((Submarine*)playerSub)->getMaxForce();
}
