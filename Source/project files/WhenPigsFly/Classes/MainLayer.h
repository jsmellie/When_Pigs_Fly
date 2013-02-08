#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "CommonHeader.h"

class Object;
class Player;
class GLESDebugDraw;
class CustomContactListener;

class MainLayer : public cocos2d::CCLayer
{
	// Fields
	// --------------------
private:
	// Refference to the player
	Player* m_pPlayer;

	// Box2D world
	b2World* m_pWorld;

	// Physics body for the level
	b2Body* m_pLevelBody;

	//Debug renderer for Box2D
	GLESDebugDraw* m_pDebugRenderer;

	// Contact listener for collision
	CustomContactListener* m_pContactListener;

	//TEMP Obstacle testing
	float m_TimePassed;
	
	// Methods
	// --------------------

	// Getters and setters
public:
	// Returns the player object
	virtual Player* getPlayer();

	// Returns the Box2D world
	virtual b2World* getWorld();

	// Returns the physics body for the level
	virtual b2Body* getLevelBody();

	// Gameloop and creation functions
public:
    // Auto implements the basic "create()" function.
	// this function requires "init()" to be made.
    CREATE_FUNC(MainLayer);
    virtual bool init();

	// Initialize default objects for this layer;
	virtual void InitObjects();

	// Update function
	void update(float delta);

	// Draw function, used for physics debug
	virtual void draw();

	//Override input callback functions
	virtual void ccTouchesBegan (cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded (cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
};

#endif // __MAIN_LAYER_H__
