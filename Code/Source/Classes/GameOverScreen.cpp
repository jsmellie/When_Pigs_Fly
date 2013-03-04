#include "GameOverScreen.h"
#include "GameScreen.h"
#include "MainMenuScreen.h"

void GameOverScreen::setTransition(GOTransition transition)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_Transition = transition;

	switch(m_Transition)
	{
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
			// Get the instance of the game screen
			CCScene* scene = MainMenuScreen::create();

			// Replace the current screen with the game screen, but use the page turn transition
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
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
	if(!MenuScreen::init())
	{
		return false;
	}

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
	if(!MenuScreen::initWithBackLayer(backLayer))
	{
		return false;
	}

	//// Set the transition to in
	setTransition(GOIn);

	// Everything was successfull
	return true;
}

GameOverScreen::~GameOverScreen()
{}

bool GameOverScreen::initButtonLayer()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Initialization of button layer
	m_pButtonLayer = CCLayer::create();

	// Set touch event's on for this layer
	m_pButtonLayer->setTouchEnabled(true);

	// Creation of the replay button
	CCMenuItemImage* pReplay = CCMenuItemImage::create(REPLAY_FILENAME, REPLAY_FILENAME, m_pButtonLayer, menu_selector(GameOverScreen::replayCallback));
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

	//this->addChild(m_pButtonLayer, 0);

	return true;
}

void GameOverScreen::update(float delta)
{

}

void GameOverScreen::mainMenuCallback(CCObject* pSender)
{
   setTransition(GOToMainMenu);
}

void GameOverScreen::replayCallback(CCObject* pSender)
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