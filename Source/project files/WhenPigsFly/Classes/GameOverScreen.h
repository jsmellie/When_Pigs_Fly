#ifndef __GAMEOVER_SCREEN_H__
#define __GAMEOVER_SCREEN_H__

#include "CommonHeader.h"

class GameOverScreen : public CCScene
{
	// Fields
	// --------------------
protected:
	CCLayer* m_Layer;

	
	// Methods
	// --------------------

	// Getters and setters
public:
	// Returns the layer in use
	CCLayer* getLayer();


	//Gameloop and creation functions
public:
	// Basic create function for GameOverScreen
    CREATE_FUNC(GameOverScreen);

	// Basic init
	bool init();


	//Button callback functions
public:
	// Callback function for the exit button
	void mainMenuCallback(CCObject* pSender);

	// Callback function for the play again button
	void playAgainCallback(CCObject* pSender);

	// Callback function for the highscore button
	void highscoreCallback(CCObject* pSender);
};

#endif //__GAMEOVER_SCREEN_H__