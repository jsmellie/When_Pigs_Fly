#ifndef __MENU_SCREEN_H__
#define __MENU_SCREEN_H__

#include "CommonHeader.h"

class MenuScreen : public CCScene
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
	
	// Methods
	// --------------------

	// Getters and setters
public:
	// Returns the background layer
	virtual CCLayer* getBackLayer();

	//Gameloop and creation functions
public:
	// Basic init
	//virtual bool init();

	// Init with a passed layer
	//virtual bool initWithBackLayer(CCLayer* backLayer);

	// Deconstructor
	virtual ~MenuScreen();

	// Initialize the button layer, will be overidden in menus
	virtual bool initButtonLayer() = 0;

	// Updates the layer
	virtual void update(float delta);
};

#endif //__MENU_SCREEN_H__