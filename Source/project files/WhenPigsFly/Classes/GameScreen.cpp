#include "GameScreen.h"
#include "AppMacros.h"
#include "MainLayer.h"
#include "ParallaxObject.h"

USING_NS_CC;


GameScreen* GameScreen::m_pInstance = NULL;




void GameScreen::getLayers(CCLayer* background, CCLayer* main)
{
	background = NULL;//getBackground();

	main = getMain();
}


CCLayer* GameScreen::getBackground()
{
	return m_pBackground;
}


MainLayer* GameScreen::getMain()
{
	return m_pMain;
}

GameScreen* GameScreen::getInstance()
{
	if(m_pInstance == NULL)
	{
		GameScreen::create();
	}

	return m_pInstance;
}

b2Vec2 GameScreen::getGravity()
{
	return m_vGravity;
}

bool GameScreen::init()
{
	if( !CCScene::init())
	{
		return false;
	}

	if(m_pInstance != NULL)
	{
		return false;
	}
	else
	{
		m_pInstance = this;
	}

	m_vGravity = b2Vec2(0.0f, -9.8f);

	// Creation of the layers
	m_pMain = MainLayer::create();
	m_pMain->retain();

	// Create the backgroud layer
	m_pBackground = CCLayer::create();
	m_pBackground->retain();

	// Initialize background with it's values
	initBackground();

	//Add the layers as children
	this->addChild(m_pMain, 0);
	this->addChild(m_pBackground, -1);
	//TODO: Add m_pBackground and m_pForeground as children

	return true;
}

bool GameScreen::initBackground()
{
	//Creation of test parallax objects
	ParallaxObject* closeParallax = ParallaxObject::create(CLOSE_BACKGROUND_FILENAME, -2);
	ParallaxObject* mediumParallax = ParallaxObject::create(MEDIUM_BACKGROUND_FILENAME, -6);
	ParallaxObject* cloudsParallax = ParallaxObject::create(FAR_BACKGROUND_FILENAME, -20);
	CCSprite* sky = CCSprite::create(SKY_FILENAME);

	sky->setPositionY(sky->getPositionY() + sky->getContentSize().height/2);
	sky->setPositionX(sky->getPositionX() + sky->getContentSize().width/2);

	m_pBackground->addChild(closeParallax, -1);
	m_pBackground->addChild(mediumParallax, -2);
	m_pBackground->addChild(cloudsParallax, -9);
	m_pBackground->addChild(sky, -10);

	return true;
}

GameScreen::~GameScreen()
{
	this->removeAllChildren();

	m_pMain->release();
	m_pBackground->release();

	m_pInstance = NULL;
}

bool GameScreen::releaseBackground()
{
	unsigned int currentRetain = m_pBackground->retainCount();

	this->removeChild(m_pBackground);

	if(m_pBackground->retainCount() <= currentRetain)
	{
		return true;
	}

	return false;
}