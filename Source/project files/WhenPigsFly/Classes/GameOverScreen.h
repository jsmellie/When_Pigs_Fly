#ifndef __GAMEOVER_SCREEN_H__
#define __GAMEOVER_SCREEN_H__

#include "CommonHeader.h"

#define TAG_GAMEOVER_FADE 9

class GameOverScreen : public CCScene
{
	// Fields
	// --------------------
protected:
	// Background layer
	CCLayer* m_pBackLayer;

	// Button layer
	CCLayer* m_pButtonLayer;

	// Bool for if the screen is fading
	bool m_IsFading;

	
	// Methods
	// --------------------

	// Getters and setters
public:
	// Returns the background layer
	CCLayer* getBackLayer();

protected:
	//Returns a bool for if the screen is fading out
	bool isFading();

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

	// Decontructor
	virtual ~GameOverScreen();

	// Initialize the button layer
	bool initButtonLayer();

	// Updates the layer
	virtual void update(float delta);


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