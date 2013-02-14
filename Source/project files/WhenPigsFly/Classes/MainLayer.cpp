#include "MainLayer.h"
#include "AppMacros.h"
#include "Player.h"
#include "Object.h"
#include "GameScreen.h"
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

	//TEST OBSTACLE MANAGER
	ObstacleManager* testOM = ObstacleManager::getInstance();

	this->addChild(testOM, 0);
}

void MainLayer::update(float delta)
{
	// If objects aren't initialized
	if(m_pPlayer == NULL || m_pWorld == NULL || m_pLevelBody == NULL || m_pDebugRenderer == NULL)
	{
		// Initialize all objects
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

		Object* objA = (Object*)(contact.fixtureA->GetBody()->GetUserData());

		if(objA->getName() == "Player")
		{
			ObstacleManager::getInstance()->collisionWithPlayer((Obstacle*)(contact.fixtureB->GetBody()->GetUserData()));
		}
		else
		{
			ObstacleManager::getInstance()->collisionWithPlayer((Obstacle*)(contact.fixtureA->GetBody()->GetUserData()));
		}
	}

	m_pContactListener->m_Contacts.clear();
}

MainLayer::~MainLayer()
{
	this->removeAllChildrenWithCleanup(true);

	m_pWorld->DestroyBody(m_pLevelBody);

	delete m_pWorld;

	m_pPlayer->release();
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
