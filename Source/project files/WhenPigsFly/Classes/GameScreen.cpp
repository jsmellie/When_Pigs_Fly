#include "GameScreen.h"
#include "AppMacros.h"
#include "MainLayer.h"
#include "DetailLayer.h"

USING_NS_CC;


GameScreen* GameScreen::m_pInstance = NULL;




void GameScreen::getLayers(CCLayer* background, CCLayer* main)
{
	background = NULL;//getBackground();

	main = getMain();
}


//DetailLayer* GameScreen::getBackground()
//{
//	return m_pBackground;
//}


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

	//TODO: Create m_pBackground here
	//m_pBackground = DetailLayer::create();

	//Add the layers as children
	this->addChild(m_pMain, 0);
	//TODO: Add m_pBackground and m_pForeground as children

	return true;
}

GameScreen::~GameScreen()
{
	m_pMain->release();
	//m_pBackground->release();

	m_pInstance = NULL;
}