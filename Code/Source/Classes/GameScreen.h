#ifndef __GAME_SCREEN_H__
#define __GAME_SCREEN_H__


#include "CommonHeader.h"

#define SCORE_PER_SECOND		2.115f

class MainLayer;

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
	CCLayer* m_pBackground;

	//Variable for the games gravity
	b2Vec2 m_vGravity;

	// Current score of the player
	float m_fScore;

	// Label to display the score
	CCLabelTTF* m_pScoreLabel;

	// Methods
	// --------------------

	// Getters and setter
public:

	//Get for m_pInstance
	static GameScreen* getInstance();
	
	//Get's the background layer
	virtual CCLayer* getBackground();

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

	// Create background layer
	virtual bool initBackground();

	virtual void update(float delta);

	virtual ~GameScreen();

	// Other usefull functions
public:
	// releases the gamescreens retain of the backgrounds
	virtual bool releaseBackground();

	// removes the game screen and adds a game over screen
	virtual void gameOver();

	// sets the score to 0 to start the incrementation
	virtual void startScore();


};

#endif // __GAME_SCREEN_H__