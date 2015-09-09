#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <unordered_map>

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void update(float);

private:
	cocos2d::Camera* camera;
    cocos2d::EventKeyboard::KeyCode lastKey;
    std::vector<cocos2d::EventKeyboard::KeyCode> activeKeys;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, float*> typeKeyCandidates;
    bool mouseDownFudge, mouseUpFudge, mouseMoveFudge, mouseScrollFudge;
    bool isMouseDown[3]; //cocos2dx only supports left right middle mouse buttons in that order

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onMouseDown(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseScroll(cocos2d::Event* event);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* e);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* e);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* e);

    void onKeyTyped(cocos2d::EventKeyboard::KeyCode keyCode);

    virtual void onEnter();
    virtual void onExit();
};

#endif // __HELLOWORLD_SCENE_H__
