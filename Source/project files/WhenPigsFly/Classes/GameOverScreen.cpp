#include "GameOverScreen.h"

CCLayer* GameOverScreen::getLayer()
{
	return m_Layer;
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

void GameOverScreen::exitCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
