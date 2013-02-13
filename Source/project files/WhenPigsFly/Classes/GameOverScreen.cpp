#include "GameOverScreen.h"

CCLayer* GameOverScreen::getLayer()
{
	return m_Layer;
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

	m_Layer = CCLayer::create();

	this->addChild(m_Layer);

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

	m_Layer = backLayer;

	this->addChild(m_Layer);

	return true;
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
