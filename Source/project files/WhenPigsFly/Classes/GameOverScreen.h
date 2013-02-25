#ifndef __GAMEOVER_SCREEN_H__
#define __GAMEOVER_SCREEN_H__

#include "CommonHeader.h"

#define TAG_GAMEOVER_FADE 9

enum GOTransition
{
	GONoTrans = 0,
	GOIn,
	GOToMainMenu,
	GOReplay,
	GOToHighscores
};

class GameOverScreen : public CCScene
{
	// Fields
	// --------------------
protected:
	// Background layer
	CCLayer* m_pBackLayer;

	// Button layer
	CCLayer* m_pButtonLayer;

	// Menu object
	CCMenu* m_pMenu;

	// Enum that reprensets what the current fade state
	GOTransition m_Transition;
	
	// Methods
	// --------------------

	// Getters and setters
public:
	// Returns the background layer
	CCLayer* getBackLayer();

protected:
	// Set the transition
	void setTransition(GOTransition transition);

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


	// Helper functions
protected:
	// Re-activates the background layer so that the background still moves
	virtual bool reactivateBack();
};

#endif //__GAMEOVER_SCREEN_H__