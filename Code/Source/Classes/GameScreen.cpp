#include "GameScreen.h"
#include "AppMacros.h"
#include "MainLayer.h"
#include "ParallaxObject.h"
#include "GameOverScreen.h"

USING_NS_CC;


GameScreen* GameScreen::m_pInstance = 0;

GameScreen* GameScreen::getInstance()
{
	if(m_pInstance == 0)
	{
		GameScreen::create();
	}

	return m_pInstance;
}

CCLayer* GameScreen::getBackground()
{
	return m_pBackground;
}

MainLayer* GameScreen::getMain()
{
	return m_pMain;
}

CCLayer* GameScreen::getGUI()
{
	return m_pGUI;
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

	if(m_pInstance != 0)
	{
		return false;
	}
	else
	{
		m_pInstance = this;
	}

	m_pMain = 0;

	m_pGUI = m_pBackground = 0;

	m_pScoreLabel = 0;

	m_vGravity = b2Vec2(0.0f, -9.8f);

	m_fScore = -1;

	// Creation of the layers
	m_pMain = MainLayer::create();
	m_pMain->retain();

	// Initialize background with it's values
	initBackground();

	// Initialize the GUI
	initGUI();

	// Check to make sure all the layers are initialized
	CCAssert(m_pMain && m_pBackground && m_pGUI, "");

	//Add the layers as children
	this->addChild(m_pMain, 0);
	this->addChild(m_pBackground, -1);
	this->addChild(m_pGUI, 1);

	this->scheduleUpdate();

	return true;
}

bool GameScreen::initBackground()
{
	// Create the backgroud layer
	m_pBackground = CCLayer::create();
	m_pBackground->retain();

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

bool GameScreen::initGUI()
{
	// Initialize the score's label
	m_pScoreLabel = CCLabelTTF::create("0 km", PAINT_FONT_FILENAME, SCORE_SIZE);

	ccColor3B color;

	color.r = color.b = color.g = 255;

	m_pScoreLabel->setColor(color);
	m_pScoreLabel->setPosition(ccp(VISIBLESIZE.width - m_pScoreLabel->getContentSize().width, VISIBLESIZE.height - m_pScoreLabel->getContentSize().height));

	m_pGUI = CCLayer::create();
	CC_SAFE_RETAIN(m_pGUI);

	m_pGUI->addChild(m_pScoreLabel);

	return true;
}

void GameScreen::update(float delta)
{
	if(m_fScore > -1)
	{
		m_fScore += delta * SCORE_PER_SECOND;

		char* scoreString = new char[16];

		sprintf(scoreString, "%.0f km", m_fScore);
		
		m_pScoreLabel->setString(scoreString);

		m_pScoreLabel->setPosition(ccp(VISIBLESIZE.width - (m_pScoreLabel->getContentSize().width/6)*5, VISIBLESIZE.height - m_pScoreLabel->getContentSize().height));
	}
}

GameScreen::~GameScreen()
{
	this->removeAllChildrenWithCleanup(true);

	CC_SAFE_RELEASE_NULL(m_pMain);
		
	CC_SAFE_RELEASE_NULL(m_pBackground);

	m_pInstance = 0;
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

void GameScreen::gameOver()
{
	// Get a reference to the background layer
	CCLayer* backLayer = getBackground();
	backLayer->retain();

	// Release the background layer from the game screen
	releaseBackground();

	// Create a game over screen and set it's back layer to the background layer
	GameOverScreen* gameOver = GameOverScreen::createWithBackLayerAndScore(GameScreen::getInstance()->getBackground(), m_fScore);

	// replace the game screen with the game over menu
	CCDirector::sharedDirector()->replaceScene(gameOver);

	// release the current reference of the background layer
	backLayer->release();
}

void GameScreen::startScore()
{
	CCAssert(m_fScore < 0,"");

	if(m_fScore < 0)
	{
		m_fScore = 0;
	}
}