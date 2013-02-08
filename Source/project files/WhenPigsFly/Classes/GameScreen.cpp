#include "GameScreen.h"
#include "AppMacros.h"
#include "MainLayer.h"
#include "DetailLayer.h"

USING_NS_CC;


GameScreen* GameScreen::m_pInstance = NULL;

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

	//TODO: Create m_pForeground here

	//Add the layers as children
	this->addChild(m_pMain, 0);
	//TODO: Add m_pBackground and m_pForeground as children

	return true;
}


void GameScreen::getLayers(CCLayer* background, CCLayer* main, CCLayer* foreground)
{
	background = NULL;//GetBackground();

	main = getMain();

	foreground = NULL;//GetForeground();
}

void GameScreen::draw()
{
	int bp = 0;
}


//DetailLayer* GameScreen::GetBackground()
//{
//	return m_pBackground;
//}


MainLayer* GameScreen::getMain()
{
	return m_pMain;
}


//DetailLayer* GameScreen::getForeground()
//{
//	return m_pForeground;
//}

GameScreen* GameScreen::getInstance()
{
	return m_pInstance;
}

b2Vec2 GameScreen::getGravity()
{
	return m_vGravity;
}