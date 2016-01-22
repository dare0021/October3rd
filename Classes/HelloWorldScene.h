#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

//#define MOUSE_DOUBLE_LISTEN_FUDGE

#include "cocos2d.h"
#include <unordered_map>
#include "Entities/O3Sprite.h"
#include "Entities/Submarine.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void update(float);

private:
    cocos2d::Sprite *overlaySprite, *cursorSprite, *protractor, *commorose, *playerSub, *notifier;
    cocos2d::EventKeyboard::KeyCode lastKey;
    cocos2d::Vec2 lastCursor, lastPlayerPos;
    std::vector<cocos2d::EventKeyboard::KeyCode> activeKeys;
    /// contains sprites not contained in other sprite vectors, e.g. torpedoVect
    std::vector<O3Sprite*> spriteVect;
    std::vector<Torpedo*> torpedoVect;
    std::vector<Submarine*> subVect;
    //std::vector<CounterMreasure*> CMVect;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, float*> typeKeyCandidates;
    ///cocos2dx mouse listener is broken
    #ifdef MOUSE_DOUBLE_LISTEN_FUDGE
    bool mouseDownFudge, mouseUpFudge, mouseMoveFudge, mouseScrollFudge;
    #endif
    bool isMouseDown[3]; //cocos2dx only supports left right middle mouse buttons in that order
    float timeSinceLastMouseUp, timeSinceLastNotifierUpdate, timeSinceLastObjectCull;

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
    void repaintCursor();

    void lookAt(cocos2d::Vec2);
    cocos2d::Vec2 lookingAt();
    cocos2d::Vec2 pointingAt();
    void moveScreenBy(cocos2d::Vec2);
    bool onScreen(cocos2d::Vec2);
    bool onScreen(cocos2d::Vec2, cocos2d::Vec2);
    bool onScreen(cocos2d::Vec2, cocos2d::Vec4);
    cocos2d::Vec2 screenspaceToWorldspace(cocos2d::Vec2);
    cocos2d::Vec2 worldspaceToScreenspace(cocos2d::Vec2);

    O3Sprite* getSpriteByName(std::string name);
    void addSprite(O3Sprite*, bool addToSpriteVect = true);
    int removeSpriteByName(std::string name);
    bool removeSpriteByID(int ID);

    void addTorpedo(Torpedo*);
    bool removeTorpedoByID(int ID);

    virtual void onEnter();
    virtual void onExit();
};

#endif // __HELLOWORLD_SCENE_H__
