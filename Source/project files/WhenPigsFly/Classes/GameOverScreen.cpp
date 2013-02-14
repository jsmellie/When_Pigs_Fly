#include "GameOverScreen.h"
#include "GameScreen.h"

CCLayer* GameOverScreen::getBackLayer()
{
	return m_pBackLayer;
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

	m_pBackLayer = NULL;//CCLayer::create();
	m_pButtonLayer = NULL;
	m_IsFading = false;

	this->addChild(m_pButtonLayer);

	this->scheduleUpdate();

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

	m_pBackLayer = NULL;
	m_pButtonLayer = NULL;
	m_IsFading = false;

	m_pBackLayer = backLayer;

	initButtonLayer();

	this->addChild(m_pBackLayer, -1);
	this->addChild(m_pButtonLayer, 0);

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
	pReplay->setPosition(visibleSize.width/2 + origin.x,visibleSize.height/2 + origin.y);

	CCMenuItemImage* pMainMenu = NULL;

	CCMenuItemImage* pHighscores = NULL;

	// Menu object that holds the buttons
	CCMenu* pMenu = CCMenu::create(pReplay, pMainMenu, pHighscores, NULL);

	pMenu->setPosition(CCPointZero);

	m_pButtonLayer->addChild(pMenu, 1);

	return false;
}

void GameOverScreen::update(float delta)
{
	/*if(this->getActionByTag(TAG_GAMEOVER_FADE) == NULL && isFading())
	{

	}*/
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
	CCScene* game = GameScreen::getInstance();

	CCDirector::sharedDirector()->replaceScene(game);
}
