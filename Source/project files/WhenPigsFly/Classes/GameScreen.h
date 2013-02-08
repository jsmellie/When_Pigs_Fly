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

	//Layer that will contain all foreground objects
	DetailLayer* m_pForeground;

	//Variable for the games gravity
	b2Vec2 m_vGravity;

	// Methods
	// --------------------
public:

	//Function to initialize everything
	virtual bool init();

	//Get for m_pInstance
	static GameScreen* getInstance();

	//Sets each respective pointer to the proper layer
	virtual void getLayers(cocos2d::CCLayer* background, cocos2d::CCLayer* main, cocos2d::CCLayer* foreground);

	//Get's the background layer
	//virtual cocos2d::DetailLayer* getBackground();

	//Get's the main layer
	virtual MainLayer* getMain();

	void draw();
	
	//Get's the foreground layer
	//virtual cocos2d::DetailLayer* getForeground();

	// Getter for m_vGravity
	virtual b2Vec2 getGravity();

	//Auto-creates the "create()" function for this object and makes it autorelease.
	CREATE_FUNC(GameScreen);


};

#endif // __GAME_SCREEN_H__