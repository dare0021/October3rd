#include "HelloWorldScene.h"

#include "Helpers/StaticHelpers.h"
#include "Helpers/Consts.h"
#include "Entities/Commorose.h"

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    overlaySprite = Sprite::create();
    overlaySprite->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(overlaySprite, 1024);

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(0,label->getContentSize().height/2));

    // add the label as a child to this layer
    overlaySprite->addChild(label, 1);

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
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    {
        auto s = getSpriteByName("dummy.png");
        if(s)
        {
            s->setPhysicsModel(O3Sprite::Newtonian);
            s->setMass(300);
            s->setFriction(5);
            s->setRotation(((int)(s->getRotation() + 45))%360);
            s->setForce(s->getForce() + 1000);
        }
        break;
    }
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    {
        auto s = getSpriteByName("dummy.png");
        if(s)
        {
            s->setForce(s->getForce() - 1000);
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

    if(e->getMouseButton() == 1 && !getChildByName("dummy.png"))
    {
        auto s = new O3Sprite("dummy.png");
        addSprite(s);
        s->addAnimation("idle", "animtest", 9, 12);
        s->setAnimation("idle");
        s->playAnimation();
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
    if(rmb)
    {//RMB drag
        CCLOG("RMB");
        moveScreenBy(-1*touch->getDelta());
    }
    if(!(rmb || mmb))
    {
        Commorose* c = (Commorose*)getChildByName("commorose");
        if(!c)
        {
            c = new Commorose("commorose");
            c->setPosition(pointingAt());
            addChild(c);
        }
        else
        {
            Vec2 delta = touch->getLocation() - touch->getStartLocation();
            if(delta.length() > COMMOROSE_MIN_DIST)
                c->setCursorAngle(StaticHelpers::headingAngle(delta));   
            else
                c->setCursorAngle(-1);
        }
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* e)
{
    Commorose* c = (Commorose*)getChildByName("commorose");
    if(c)
    {
        switch (c->getMode()) //TODO: do something with this
        {
            case 0:
            {
                break;
            }
            case 1:
            {
                break;
            }
            case 2:
            {
                break;
            }
            case 3:
            {
                break;
            }
        }
        removeChildByName("commorose");
    }
}

void HelloWorld::lookAt(Vec2 pos)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->setPosition(pos - Vec2(visibleSize.width, visibleSize.height)/2);
    CCLOG("NOW LOOKING AT: (%f %f)", lookingAt().x, lookingAt().y);
}

Vec2 HelloWorld::lookingAt()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return -1 * this->getPosition() + Vec2(visibleSize.width, visibleSize.height)/2;
}

Vec2 HelloWorld::pointingAt()
{
    return -1 * this->getPosition() + lastCursor;
}

void HelloWorld::moveScreenBy(Vec2 diff)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    lookAt(this->getPosition() + Vec2(visibleSize.width, visibleSize.height)/2 - diff);
}

Vec2 HelloWorld::screenspaceToWorldspace(Vec2 sspos)
{
    Vec2 origin = this->getPosition();
    return sspos + origin;
}

Vec2 HelloWorld::worldspaceToScreenspace(Vec2 wspos)
{
    Vec2 origin = this->getPosition();
    return wspos - origin;
}

void HelloWorld::update(float dt)
{
    for (auto p : typeKeyCandidates)
    {
        *p.second += dt;
    }

    for (auto o3s : spriteVect)
    {
        o3s->update(dt);
    }
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

void HelloWorld::addSprite(O3Sprite* sprite)
{
    spriteVect.push_back(sprite);
    this->addChild(sprite);
}

void HelloWorld::removeSprite(O3Sprite* sprite)
{
    std::list<int> toRemove;
    int i = 0;
    for (auto s : spriteVect)
    {
        if(s == sprite)
            toRemove.push_front(i);
        i++;
    }
    for (auto j : toRemove)
    {
        this->removeChild(*(spriteVect.begin() + j));
        spriteVect.erase(spriteVect.begin() + j);
    }
}

void HelloWorld::removeSpriteByName(std::string name)
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
}