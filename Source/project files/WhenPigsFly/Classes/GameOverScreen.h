#ifndef __GAMEOVER_SCREEN_H__
#define __GAMEOVER_SCREEN_H__

#include "CommonHeader.h"

class GameOverScreen : public CCScene
{
	// Fields
	// --------------------
protected:
	// Background layer
	CCLayer* m_BackLayer;

	// 

	
	// Methods
	// --------------------

	// Getters and setters
public:
	// Returns the background layer


	//Gameloop and creation functions
public:
	// Creating a basic object
	static GameOverScreen* create();
	// Basic init
	bool init();

	// Creating a new object with a background layer
    static GameOverScreen* createWithBackLayer(CCLayer* backLayer);
	// Init with a passed layer
	bool initWithBackLayer(CCLayer* backLayer);


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