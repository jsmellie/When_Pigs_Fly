#ifndef __GAME_SCREEN_H__
#define __GAME_SCREEN_H__


#include "CommonHeader.h"

class MainLayer;
class DetailLayer;

//TODO: Think of a better name then DetailLayer that can mean both foreground and background

class GameScreen : public cocos2d::CCScene
{
	// Fields
	// --------------------
private:

	//Static instance of GameScreen
	static GameScreen* m_pInstance;

	//Layer that will contain all collidable objects
	MainLayer* m_pMain;

	//Layer that will contain all paralax & background objects
	DetailLayer* m_pBackground;

	//Variable for the games gravity
	b2Vec2 m_vGravity;

	// Methods
	// --------------------

	// Getters and setter
public:

	//Get for m_pInstance
	static GameScreen* getInstance();
	
	//Get's the background layer
	//virtual DetailLayer* getBackground();

	//Get's the main layer
	virtual MainLayer* getMain();

	//Sets each respective pointer to the proper layer
	virtual void getLayers(cocos2d::CCLayer* background, cocos2d::CCLayer* main);

	// Getter for m_vGravity
	virtual b2Vec2 getGravity();

	// Gameloop, creation and destruction
public:
	//Auto-creates the "create()" function for this object and makes it autorelease.
	CREATE_FUNC(GameScreen);

	//Function to initialize everything
	virtual bool init();

	virtual ~GameScreen();
};

#endif // __GAME_SCREEN_H__