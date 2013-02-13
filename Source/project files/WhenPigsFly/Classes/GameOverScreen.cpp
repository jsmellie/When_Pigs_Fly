#include "GameOverScreen.h"

CCLayer* GameOverScreen::getBackLayer()
{
	return m_BackLayer;
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
	return NULL;
}

bool GameOverScreen::init()
{
	if(!CCScene::init())
	{
		return false;
	}

	m_BackLayer = NULL;//CCLayer::create();

	m_ButtonLayer = CCLayer::create();

	this->addChild(m_ButtonLayer);

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
	return NULL;
}

bool GameOverScreen::initWithBackLayer(CCLayer* backLayer)
{
	if(!CCScene::init())
	{
		return false;
	}

	m_BackLayer = NULL;
	m_ButtonLayer = NULL;

	return true;
}

bool GameOverScreen::initButtonLayer()
{
	// Initialization of button layer
	m_ButtonLayer = CCLayer::create();

	// Creation of the replay button
	CCMenuItemImage* pReplay = CCMenuItemImage::create(REPLAY_FILENAME, REPLAY_FILENAME, m_ButtonLayer, menu_selector(GameOverScreen::playAgainCallback));

	CCMenuItemImage* pMainMenu = NULL;

	CCMenuItemImage* pHighscores = NULL;

	// Menu object that holds the buttons
	CCMenu* pMenu = CCMenu::create(pReplay, pMainMenu, pHighscores, NULL);

	pMenu->setPosition(CCPointZero);

	m_ButtonLayer->addChild(pMenu, 1);

	return false;
}

void GameOverScreen::mainMenuCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameOverScreen::playAgainCallback(CCObject* pSender)
{

}
