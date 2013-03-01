#ifndef __GAMEOVER_SCREEN_H__
#define __GAMEOVER_SCREEN_H__

#include "CommonHeader.h"
#include "MenuScreen.h"

#define TAG_GAMEOVER_FADE 9

enum GOTransition
{
	GOIn = 0,
	GOToMainMenu,
	GOReplay,
	GOToHighscores
};

class GameOverScreen : public MenuScreen
{
	// Fields
	// --------------------
protected:
	// Enum that reprensets what the current fade state
	GOTransition m_Transition;
	
	// Methods
	// --------------------

	// Getters and setters
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
	virtual bool initButtonLayer();

	// Updates the layer
	virtual void update(float delta);


	//Button callback functions
public:
	// Callback function for the main menu button
	void mainMenuCallback(CCObject* pSender);

	// Callback function for the replay button
	void replayCallback(CCObject* pSender);

	// Callback function for the highscore button
	void highscoreCallback(CCObject* pSender);


	// Helper functions
protected:
	// Re-activates the background layer so that the background still moves
	virtual bool reactivateBack();
};

#endif //__GAMEOVER_SCREEN_H__