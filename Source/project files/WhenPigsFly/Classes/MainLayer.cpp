#include "MainLayer.h"
#include "AppMacros.h"
#include "Player.h"
#include "Object.h"
#include "GameScreen.h"
#include "ParallaxObject.h"
#include "GLES-Render.h"
#include "ObstacleManager.h"
#include "CustomContactListener.h"

USING_NS_CC;


Player* MainLayer::getPlayer()
{
	return m_pPlayer;
}

b2World* MainLayer::getWorld()
{
	return m_pWorld;
}

b2Body* MainLayer::getLevelBody()
{
	return m_pLevelBody;
}

// on "init" you need to initialize your instance
bool MainLayer::init()
{
	//base class init
    if ( !CCLayer::init() )
    {
        return false;
    }

	//Enable touch input for this layer
	this->setTouchEnabled(true);

	m_pPlayer = NULL;
	m_pWorld = NULL;
	m_pLevelBody = NULL;
	m_pDebugRenderer = NULL;
	m_TimePassed = 0;
	m_pContactListener = NULL;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


#pragma region DEFAULT STUFF

 //   // add a "close" icon to exit the progress. it's an autorelease object
	//CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
 //                                       "CloseNormal.png",
 //                                       "CloseSelected.png",
 //                                       this,
 //                                       menu_selector(MainLayer::menuCloseCallback));
 //   
	//pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
 //                               origin.y + pCloseItem->getContentSize().height/2));

 //   // create menu, it's an autorelease object
 //   CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
 //   pMenu->setPosition(CCPointZero);
 //   this->addChild(pMenu, 1);

#pragma endregion

	this->scheduleUpdate();

    return true;
}

void MainLayer::InitObjects()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//Creation of the Box2D world
	m_pWorld = new b2World(GameScreen::getInstance()->getGravity());

	//Initialization of the debug renderer
	m_pDebugRenderer = new GLESDebugDraw(PTM_RATIO);
	//Attaching it to the physics world
	m_pWorld->SetDebugDraw(m_pDebugRenderer);

	//Set the flags that will represent specifically what will draw
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	m_pDebugRenderer->SetFlags(flags);

	//Initialize the contact listener
	m_pContactListener = new CustomContactListener();
	//Attaching it to the world
	m_pWorld->SetContactListener(m_pContactListener);

	//Body deffinition for the level
	b2BodyDef levelBobyDef;
	levelBobyDef.position.Set(0,0);

	//Creation of the level's physics body
	m_pLevelBody = m_pWorld->CreateBody(&levelBobyDef);

	//Creation of the shape of the bottum
	b2EdgeShape bottumEdge;

	//Creation of the deffinition of the fixture
	b2FixtureDef boxShapeDef;
	//Attaching the shape to the fixture
	boxShapeDef.shape = &bottumEdge;

	//Setting the edge as the bottum edge
	bottumEdge.Set(b2Vec2(0,0), b2Vec2((visibleSize.width + origin.x)/PTM_RATIO, 0));
	//Adding the fixture to the body.
	m_pLevelBody->CreateFixture(&boxShapeDef);

	// Creation of the shape of the top
	b2EdgeShape topEdge;

	// Creation of top's fixture deffinition
	b2FixtureDef topShapeDef;
	//Attaching the shape to the fixture definition
	topShapeDef.shape = &topEdge;

	//Setting the shapes parameters
	topEdge.Set(b2Vec2(0, (visibleSize.height + origin.y)/PTM_RATIO), b2Vec2((visibleSize.width + origin.x)/PTM_RATIO, (visibleSize.height + origin.y)/PTM_RATIO));

	//Adding the top fixture to the level's physics body
	m_pLevelBody->CreateFixture(&topShapeDef);

	//Initialization of the player
	m_pPlayer = Player::create();
	m_pPlayer->retain();

	this->addChild(m_pPlayer, 1);

	//Creation of test parallax objects
	ParallaxObject* closeParallax = ParallaxObject::create("Backgrounds/Fields-Close.png", -2);
	ParallaxObject* mediumParallax = ParallaxObject::create("Backgrounds/Forest-Medium.png", -6);
	ParallaxObject* cloudsParallax = ParallaxObject::create("Backgrounds/Bunched-Clouds.png", -20);
	CCSprite* sky = CCSprite::create("Backgrounds/Sunset-Sky.png");

	sky->setPositionY(sky->getPositionY() + sky->getContentSize().height/2);
	sky->setPositionX(sky->getPositionX() + sky->getContentSize().width/2);

	this->addChild(closeParallax, -1);
	this->addChild(mediumParallax, -2);
	this->addChild(cloudsParallax, -9);
	this->addChild(sky, -10);

	//TEST OBSTACLE MANAGER
	ObstacleManager* testOM = ObstacleManager::getInstance();

	this->addChild(testOM, 0);
}

void MainLayer::update(float delta)
{
	if(m_pPlayer == NULL || m_pWorld == NULL || m_pLevelBody == NULL || m_pDebugRenderer == NULL)
	{
		InitObjects();
	}

	//Simulate physics
	m_pWorld->Step(delta, 10, 10);

	m_TimePassed += delta;

	if(m_TimePassed >= 1)
	{
		m_TimePassed -= 1;
		ObstacleManager::getInstance()->newObstacle();
	}

	//check contacts
	vector<CustomContact>::iterator pos;

	vector<CustomContact>::iterator endPos = m_pContactListener->m_Contacts.end();

	for(pos = m_pContactListener->m_Contacts.begin(); pos != endPos; ++pos)
	{
		CustomContact contact = *pos;

		CCLog("Collision!!");

		CCParticleSystem* fireExplosion = CCParticleExplosion::createWithTotalParticles(200);

		fireExplosion->setPosition(ccp(0,0));
		fireExplosion->setLife(0.7f);
		fireExplosion->setLifeVar(0.1f);

		ccColor4F startColor;// color of particles
        startColor.r = 0.76f;
        startColor.g = 0.25f;
        startColor.b = 0.12f;
        startColor.a = 1.0f;

		ccColor4F endColor;
        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 0.0f;

		fireExplosion->setStartColor(startColor);
		fireExplosion->setStartColorVar(endColor);
		fireExplosion->setEndColor(endColor);
		fireExplosion->setEndColorVar(endColor);

		m_pPlayer->addChild(fireExplosion, 10);

		CCParticleSystem* smokeExplosion = CCParticleExplosion::createWithTotalParticles(200);

		smokeExplosion->setTexture(CCTextureCache::sharedTextureCache()->addImage(SMOKE_FILENAME));

		smokeExplosion->setGravity(ccp(0, 100));
		
		smokeExplosion->setPosition(ccp(0,0));
		smokeExplosion->setLife(1.0f);
		smokeExplosion->setLifeVar(0.1f);

        startColor.r = 0.6f;
        startColor.g = 0.6f;
        startColor.b = 0.6f;
        startColor.a = 1.0f;

        endColor.r = 0.0f;
        endColor.g = 0.0f;
        endColor.b = 0.0f;
        endColor.a = 0.0f;

		smokeExplosion->setStartColor(startColor);
		smokeExplosion->setStartColorVar(endColor);
		smokeExplosion->setEndColor(endColor);
		smokeExplosion->setEndColorVar(endColor);

		m_pPlayer->addChild(smokeExplosion, 8);
	}

	m_pContactListener->m_Contacts.clear();
}

void MainLayer::draw()
{
#if _DEBUG

	CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    m_pWorld->DrawDebugData();

    kmGLPopMatrix();

    CHECK_GL_ERROR_DEBUG();
#endif
}

//Input callbacks
void MainLayer::ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent)
{
	m_pPlayer->TouchBegin();
}
void MainLayer::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent)
{
	m_pPlayer->TouchEnded();
}


void MainLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
