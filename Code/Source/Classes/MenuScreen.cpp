#include "MenuScreen.h"
#include "GameScreen.h"


CCLayer* MenuScreen::getBackLayer()
{
	return m_pBackLayer;
}

bool MenuScreen::init()
{
	if(!CCScene::init())
	{
		return false;
	}

	m_pBackLayer = 0;
	m_pButtonLayer = 0;

	// If the buttons didn't init properly
	if(!initButtonLayer())
	{
		return false;
	}

	this->addChild(m_pButtonLayer);

	this->scheduleUpdate();

	return true;
}

bool MenuScreen::initWithBackLayer(CCLayer* backLayer)
{
	// If CCScene doesn't init properly
	if(!CCScene::init())
	{
		return false;
	}

	// Default settings for everything
	m_pBackLayer = 0;
	m_pButtonLayer = 0;

	// If the layer passed in was null
	if(backLayer == 0)
	{
		return false;
	}

	// Set backlayer to the layer passed in
	m_pBackLayer = backLayer;

	// If the back layer is null
	if (m_pBackLayer == 0)
	{
		return false;
	}

	// If the buttons didn't init properly
	if(!initButtonLayer())
	{
		return false;
	}

	// Add the layers as children
	this->addChild(m_pBackLayer, -1);
	this->addChild(m_pButtonLayer, 0);

	// Everything was successfull
	return true;
}

MenuScreen::~MenuScreen()
{
	this->removeAllChildrenWithCleanup(true);
}

void MenuScreen::update(float delta)
{
}