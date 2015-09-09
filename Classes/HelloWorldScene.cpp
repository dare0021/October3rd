#include "HelloWorldScene.h"

#include "Helpers/StaticHelpers.h"
#include "Helpers/Consts.h"

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

    scrollTimeLeft = -1;

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
    typeKeyCandidates.insert(std::pair<EventKeyboard::KeyCode, float*>(keyCode, new float(0)));
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
    if(toRemove < activeKeys.size())
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
        moveScreenBy(Vec2(0, 10));
        break;
    }
    }
    CCLOG("TYPED: %c", StaticHelpers::keycodeToChar(keyCode));
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
        addSprite(new O3Sprite("dummy.png"));
    else if(e->getMouseButton() == 2)
        removeSpriteByName("dummy.png");
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
        scrollOut = true;
        scrollTimeLeft = SCALE_SCROLL_INTERVAL;
    }
    else if(e->getScrollY() < 0)
    {
        scrollOut = false;
        scrollTimeLeft = SCALE_SCROLL_INTERVAL;
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* e)
{
    return true;
}

///Only works for LMB drag, but at least it works
void HelloWorld::onTouchMoved(Touch* touch, Event* e)
{
    moveScreenBy(-1*touch->getDelta());

    if(isMouseDown[1])
    {//RMB drag
        CCLOG("RMB");
    }
    if(isMouseDown[2])
    {//MMB drag
        CCLOG("MMB");
    }
}

void HelloWorld::onTouchEnded(Touch* touch, Event* e)
{

}

void HelloWorld::lookAt(Vec2 pos)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->setPosition(pos - Vec2(visibleSize.width, visibleSize.height)/2);
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
    if(scrollTimeLeft > 0)
    {
        float scaleMod = SCALE_DELTA * dt / SCALE_SCROLL_INTERVAL;
        scaleMod = scrollOut ? scaleMod*-1 : scaleMod;
        this->setScale(this->getScale() + scaleMod);
        if(this->getScale() < SCALE_MIN)
            this->setScale(SCALE_MIN);
        else if(this->getScale() > SCALE_MAX)
            this->setScale(SCALE_MAX);
        scrollTimeLeft -= dt;
    }
    for (auto p : typeKeyCandidates)
    {
        *p.second += dt;
    }
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