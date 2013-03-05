#include "MainMenuScreen.h"
#include "GameScreen.h"


void MainMenuScreen::setTransition(MMTransition transition)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	float middle = (visibleSize.width/2) + origin.x;

	m_eTransition = transition;

	switch(m_eTransition)
	{
	case MMIn:
		{
			if(m_pMenu != 0)
			{
				// Set the button layer's scale to something really small
				m_pMenu->setScale(0.01f);

				// Scale the layer to it's proper size over 1 second
				CCScaleTo* pScaleTo = CCScaleTo::create(1.1f, 1);

				// Make the scale bounce nicely
				CCEaseBounceOut* pBounceScale = CCEaseBounceOut::create(pScaleTo);

				// Run the intro action
				m_pMenu->runAction(pBounceScale);
			}

			if(m_pTitle != 0)
			{
				CCMoveTo* pMoveTo = CCMoveTo::create(1.1f, ccp(middle, (visibleSize.height/4)*3 + origin.y - m_pTitle->getContentSize().height/8));

				CCEaseBounceOut* pTitleBouce = CCEaseBounceOut::create(pMoveTo);

				m_pTitle->runAction(pTitleBouce);
			}

			break;
		}
	case MMToOptions:
		{
			break;
		}
	case MMPlay:
		{
			// Get the instance of the game screen
			CCScene* scene = GameScreen::getInstance();

			// Replace the current screen with the game screen, but use the page turn transition
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
			break;
		}
	case MMToHighscores:
		{
			break;
		}

	default:
		{
			throw exception("Uncompatible enum type entered");
		}
	}
}

bool MainMenuScreen::init()
{
	if(!MenuScreen::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_pBackLayer = CCLayer::create();

	m_pBackLayer->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	CCSprite* pBackground = CCSprite::create(MAINMENU_BACKGROUND_FILENAME);

	m_pBackLayer->addChild(pBackground);

	this->addChild(m_pBackLayer, -1);

	setTransition(MMIn);

	return true;
}

MainMenuScreen::~MainMenuScreen()
{

}

bool MainMenuScreen::initButtonLayer()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Quick calculation for the middle of the screen on X
	float middle = (visibleSize.width/2) + origin.x;

	// Initialization of button layer
	m_pButtonLayer = CCLayer::create();

	// Set touch event's on for this layer
	m_pButtonLayer->setTouchEnabled(true);

	//Creation of the play button
	CCMenuItemImage* pPlay = CCMenuItemImage::create(PLAY_FILENAME, PLAY_FILENAME, m_pButtonLayer, menu_selector(MainMenuScreen::playCallback));
	pPlay->setPosition(middle - (pPlay->getContentSize().width * 1.5f), (visibleSize.height/4) + origin.y);

	//Creation of the options button
	CCMenuItemImage* pOptions = CCMenuItemImage::create(OPTIONS_FILENAME, OPTIONS_FILENAME, m_pButtonLayer, menu_selector(MainMenuScreen::optionsCallback));
	pOptions->setPosition(middle + (pPlay->getContentSize().width * 1.5f), (visibleSize.height/4) + origin.y);

	// Creation of the highscores button
	CCMenuItemImage* pHighscores = CCMenuItemImage::create(OPTIONS_FILENAME, OPTIONS_FILENAME, m_pButtonLayer, menu_selector(MainMenuScreen::optionsCallback));
	pHighscores->setPosition(middle, (visibleSize.height/4) + origin.y);

	// Menu object that holds the buttons
	m_pMenu = CCMenu::create(pPlay, pOptions, pHighscores, 0);
	
	// Set menu to be at (0,0) so that the buttons are placed properly
	m_pMenu->setPosition(CCPointZero);

	// Add menu as a child to the button layer
	m_pButtonLayer->addChild(m_pMenu, 1);

	//createTitle();

	m_pTitle = CCSprite::create(TITLE_FILENAME);

	//float middle = (visibleSize.width/2) + origin.x;

	m_pTitle->setPosition(ccp(middle, visibleSize.height + origin.y + m_pTitle->getContentSize().height));

	m_pButtonLayer->addChild(m_pTitle);

	return true;
}

void MainMenuScreen::update(float delta)
{

}

void MainMenuScreen::playCallback(CCObject* pSender)
{
	setTransition(MMPlay);
}

void MainMenuScreen::optionsCallback(CCObject* pSender)
{
	setTransition(MMToOptions);
}

void MainMenuScreen::highscoreCallback(CCObject* pSender)
{
	setTransition(MMToHighscores);
}

bool MainMenuScreen::reactivateBack()
{
	if(m_pBackLayer->isRunning() == false)
	{
		m_pBackLayer->onEnter();
	}

	bool temp = m_pBackLayer->isRunning();

	int bp = 0;

	return true;
}

void MainMenuScreen::createTitle()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_pTitle = CCSprite::create(TITLE_FILENAME);

	float middle = (visibleSize.width/2) + origin.x;

	m_pTitle->setPosition(ccp(middle, visibleSize.height + origin.y + m_pTitle->getContentSize().height));

	CCMoveTo* pMoveTo = CCMoveTo::create(1, ccp(middle, (visibleSize.height/4)*3 + origin.y - m_pTitle->getContentSize().height/8));

	CCEaseBounceOut* pTitleBouce = CCEaseBounceOut::create(pMoveTo);

	m_pTitle->runAction(pTitleBouce);

	m_pButtonLayer->addChild(m_pTitle);
}
