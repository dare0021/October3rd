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
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	//Set camera
	this->setCameraMask((unsigned short)CameraFlag::USER2, true);
	camera = Camera::createPerspective(60,
		(float)visibleSize.width / visibleSize.height, 1.0, 1000);
	camera->setCameraFlag(CameraFlag::USER2);
	//the calling order matters, we should first call setPosition3D, 
	//then call lookAt.
	camera->setPosition3D(sprite->getPosition3D() + Vec3(0, 0, 800));
	camera->lookAt(sprite->getPosition3D());
	this->addChild(camera);
    //end camera

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

    mouseDownFudge = false;
    mouseUpFudge = false;
    mouseMoveFudge = false;
    mouseScrollFudge = false;
    isMouseDown[0] = false;
    isMouseDown[1] = false;
    isMouseDown[2] = false;
    typeKeyCandidates.clear();
}

void HelloWorld::onExit()
{
    Layer::onExit();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    lastKey = keyCode;
    activeKeys.push_back(keyCode);
    typeKeyCandidates.push_back(new std::pair<EventKeyboard::KeyCode, float>(keyCode, 0));
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
    toRemove = 0;
    for (auto p : typeKeyCandidates)
    {
        if(p->first == keyCode)
            break;
        toRemove++;
    }
    if(toRemove < typeKeyCandidates.size())
	{
		auto iter = *(typeKeyCandidates.begin() + toRemove);
		if (iter->second <= TYPE_TIME_MAX)
			onKeyTyped(keyCode);
        typeKeyCandidates.erase(typeKeyCandidates.begin()+toRemove);
    }
}

///Pseudo-listener
void HelloWorld::onKeyTyped(EventKeyboard::KeyCode keyCode)
{
    CCLOG("TYPED: %c", StaticHelpers::keycodeToChar(keyCode));
}

void HelloWorld::onMouseDown(Event* event)
{
    mouseDownFudge = !mouseDownFudge;
    if(!mouseDownFudge)
        return;
    EventMouse* e = (EventMouse*)event;
    isMouseDown[e->getMouseButton()] = true;
    std::stringstream ss;
    ss << "Mouse Down detected, Key: ";
    ss << e->getMouseButton();
}

void HelloWorld::onMouseUp(Event* event)
{
    mouseUpFudge = !mouseUpFudge;
    if(!mouseUpFudge)
        return;
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
    mouseMoveFudge = !mouseMoveFudge;
    if(!mouseMoveFudge)
        return;
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

bool HelloWorld::onTouchBegan(Touch* touch, Event* e)
{
    return true;
}

///Only works for LMB drag, but at least it works
void HelloWorld::onTouchMoved(Touch* touch, Event* e)
{
    CCASSERT(isMouseDown[0], "onTouchMoved should only trigger on LMB drag by cocos2dx specifications");
    camera->setPosition3D(camera->getPosition3D() - Vec3(touch->getDelta().x, touch->getDelta().y, 0));
    
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

///Input is 1 or -1
void HelloWorld::onMouseScroll(Event* event)
{
    mouseScrollFudge = !mouseScrollFudge;
    if(!mouseScrollFudge)
        return;
    EventMouse* e = (EventMouse*)event;
    std::stringstream ss;
    ss << "Mouse Scroll detected, X: ";
    ss << e->getScrollX() << " Y: " << e->getScrollY();
}

void HelloWorld::update(float dt)
{
    for (auto p : typeKeyCandidates)
    {
        p->second += dt;
    }
}