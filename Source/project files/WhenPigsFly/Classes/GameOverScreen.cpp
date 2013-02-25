#include "GameOverScreen.h"
#include "GameScreen.h"


CCLayer* GameOverScreen::getBackLayer()
{
	return m_pBackLayer;
}

void GameOverScreen::setTransition(GOTransition transition)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_Transition = transition;

	switch(m_Transition)
	{
	case GONoTrans:
		{
			// Do nothing because there is no transition currently
			break;
		}

	case GOIn:
		{
			// Set the button layer to be high up in the air
			m_pButtonLayer->setPositionY(visibleSize.height + origin.y);

			// Create the move to action
			CCMoveTo* pMoveTo = CCMoveTo::create(1, CCPointZero);

			// Create the bounce in action
			CCEaseBounceOut* pMenuBounce = CCEaseBounceOut::create(pMoveTo);

			// Run the bounce action
			m_pButtonLayer->runAction(pMenuBounce);

			break;
		}

	case GOToMainMenu:
		{

			break;
		}

	case GOReplay:
		{
			// Get the instance of the game screen
			CCScene* scene = GameScreen::getInstance();

			// Replace the current screen with the game screen, but use the page turn transition
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
			break;
		}

	case GOToHighscores:
		{
			
			break;
		}
	default:
		{
			throw exception("Uncompatible enum type entered");
		}
	}
}

GameOverScreen* GameOverScreen::create()
{
	GameOverScreen* pobGOScreen = new GameOverScreen();

	if(pobGOScreen && pobGOScreen->init())
	{
		pobGOScreen->autorelease();
		return pobGOScreen;
	}

	CC_SAFE_DELETE(pobGOScreen);
	return 0;
}

bool GameOverScreen::init()
{
	if(!CCScene::init())
	{
		return false;
	}

	m_pBackLayer = 0;//CCLayer::create();
	m_pButtonLayer = 0;

	// If the buttons didn't init properly
	if(!initButtonLayer())
	{
		return false;
	}

	this->addChild(m_pButtonLayer);

	this->scheduleUpdate();

	setTransition(GOIn);

	return true;
}

GameOverScreen* GameOverScreen::createWithBackLayer(CCLayer* backLayer)
{
	GameOverScreen* pobGOScreen = new GameOverScreen();

	if(pobGOScreen && pobGOScreen->initWithBackLayer(backLayer))
	{
		pobGOScreen->autorelease();
		return pobGOScreen;
	}

	CC_SAFE_DELETE(pobGOScreen);
	return 0;
}

bool GameOverScreen::initWithBackLayer(CCLayer* backLayer)
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

	// Set the transition to in
	setTransition(GOIn);

	// Reactivate the background so that it's still moving
	reactivateBack();

	// Everything was successfull
	return true;
}

GameOverScreen::~GameOverScreen()
{
	this->removeAllChildrenWithCleanup(true);
}

bool GameOverScreen::initButtonLayer()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Initialization of button layer
	m_pButtonLayer = CCLayer::create();

	// Set touch event's on for this layer
	m_pButtonLayer->setTouchEnabled(true);

	// Creation of the replay button
	CCMenuItemImage* pReplay = CCMenuItemImage::create(REPLAY_FILENAME, REPLAY_FILENAME, m_pButtonLayer, menu_selector(GameOverScreen::playAgainCallback));
	pReplay->setPosition((visibleSize.width/4) + origin.x,visibleSize.height/3 + origin.y);

	// Creation of the return button
	CCMenuItemImage* pReturn = CCMenuItemImage::create(BACKTOMAIN_FILENAME, BACKTOMAIN_FILENAME, m_pButtonLayer, menu_selector(GameOverScreen::mainMenuCallback));
	pReturn->setPosition((visibleSize.width/4)*3 + origin.x,visibleSize.height/3 + origin.y);

	// Creation of the highscore button
	CCMenuItemImage* pHighscores = 0;

	// Menu object that holds the buttons
	m_pMenu = CCMenu::create(pReplay, pReturn, pHighscores, 0);

	// Set menu to be at (0,0) so that the buttons are placed properly
	m_pMenu->setPosition(CCPointZero);

	// Add menu as a child to the button layer
	m_pButtonLayer->addChild(m_pMenu, 1);

	return true;
}

void GameOverScreen::update(float delta)
{
}

void GameOverScreen::mainMenuCallback(CCObject* pSender)
{
   setTransition(GOToMainMenu);
}

void GameOverScreen::playAgainCallback(CCObject* pSender)
{
   setTransition(GOReplay);
}

void GameOverScreen::highscoreCallback(CCObject* pSender)
{
   setTransition(GOToHighscores);
}

bool GameOverScreen::reactivateBack()
{
	if(m_pBackLayer->isRunning() == false)
	{
		m_pBackLayer->onEnter();
	}

	bool temp = m_pBackLayer->isRunning();

	int bp = 0;

	return true;
}