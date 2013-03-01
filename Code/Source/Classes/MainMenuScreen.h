#ifndef __MAIN_MENU_SCREEN_H__
#define __MAIN_MENU_SCREEN_H__

#include "CommonHeader.h"
#include "MenuScreen.h"

enum MMTransition
{
	MMIn = 0,
	MMToOptions,
	MMPlay,
	MMToHighscores
};

class MainMenuScreen : public MenuScreen
{
	// Fields
	// --------------------
protected:
	// Title
	CCSprite* m_pTitle;

	// Enum that represents the current fade state
	MMTransition m_eTransition;	

	
	// Methods
	// --------------------

	// Getters and setters
protected:
	// Set the transition
	void setTransition(MMTransition transition);

	//Gameloop and creation functions
public:
	// Implementation of the basic "create()" function
	CREATE_FUNC(MainMenuScreen);
	// Basic init
	bool init();

	// Default deconstructor
	virtual ~MainMenuScreen();

	// Initialize the button layer
	virtual bool initButtonLayer();

	// Updates the layer
	virtual void update(float delta);


	//Button callback functions
public:
	// Callback function for the options button
	void optionsCallback(CCObject* pSender);

	// Callback function for the play button
	void playCallback(CCObject* pSender);

	// Callback function for the highscore button
	void highscoreCallback(CCObject* pSender);


	// Helper functions
protected:
	// Re-activates the background layer so that the background still moves
	virtual bool reactivateBack();
};

#endif //__MAIN_MENU_SCREEN_H__