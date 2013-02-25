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

	// Float that contains the time that has passed since the last obstacle was spawned
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
	// this function requires an "init()".
    CREATE_FUNC(MainLayer);
    virtual bool init();

	// Initialize default objects for this layer;
	virtual void initObjects();

	// Initialize the player.  Not done in initObjects because it is only done after the transitions are done
	virtual void initPlayer();
	
	// Activation of the obstacles
	virtual void activateObstacles();

	// Update function
	void update(float delta);

	// Draw function, used for physics debug
	virtual void draw();

	// Base deconstructor
	virtual ~MainLayer();

	// Called when the object becomes active, BEFORE the end of the trasition
	virtual void onEnter();
	//
	//// Called when the object becomes active, AFTER the end of the trasition
	virtual void onEnterTransitionDidFinish();

	//// Called when the object leaves the currently active scene
	virtual void onExit();



	// Touch callbacks
public:
	//Override input callback functions
	virtual void ccTouchesBegan (cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded (cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
};

#endif // __MAIN_LAYER_H__
