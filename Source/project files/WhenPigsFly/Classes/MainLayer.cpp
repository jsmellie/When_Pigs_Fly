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

	uint* ref = &m_uReference;

	//Enable touch input for this layer
	this->setTouchEnabled(true);

	//Set default values for all variables
	m_pPlayer = 0;
	m_pWorld = 0;
	m_pLevelBody = 0;
	m_pDebugRenderer = 0;
	m_TimePassed = 0;
	m_pContactListener = 0;

	this->scheduleUpdate();

    return true;
}

void MainLayer::initObjects()
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
}

void MainLayer::initPlayer()
{
	//Initialization of the player
	m_pPlayer = Player::create();
	m_pPlayer->retain();

	// Add the player as a child
	this->addChild(m_pPlayer, 1);
}

void MainLayer::activateObstacles()
{
	this->addChild(ObstacleManager::getInstance());
}

void MainLayer::update(float delta)
{
	//Simulate physics
	this->m_pWorld->Step(delta, 10, 10);

	m_TimePassed += delta;

	// If the time passed is more then 1 second
	if(m_TimePassed >= 1)
	{
		// Remove 1 second and spawn a new obstacle
		m_TimePassed -= 1;
		ObstacleManager::getInstance()->newObstacle();
	}

	// Get an iterator for the collision ocntacts
	vector<CustomContact>::iterator pos;

	// Loop through all contacts
	for(pos = m_pContactListener->m_Contacts.begin(); pos != m_pContactListener->m_Contacts.end(); ++pos)
	{
		// Pointer to the current contact
		CustomContact contact = *pos;

#ifdef DEBUG
		// If in debug, output a collision message
		CCLog("Collision!!");
#endif
		// Reference to object a
		Object* objA = (Object*)(contact.fixtureA->GetBody()->GetUserData());

		// if object a is the player
		if(objA->getName() == "Player")
		{
			// Tell obstacle manager that object b has collided with the player
			ObstacleManager::getInstance()->collisionWithPlayer((Obstacle*)(contact.fixtureB->GetBody()->GetUserData()));
		}
		else
		{
			// Tell obstacle manager that object a has collided with the player
			ObstacleManager::getInstance()->collisionWithPlayer((Obstacle*)(contact.fixtureA->GetBody()->GetUserData()));
		}
	}

	// Clear the contact list because we've assest all the collisions
	m_pContactListener->m_Contacts.clear();
}

void MainLayer::draw()
{
#ifdef _DEBUG
	// Draw the layer properly
	CCLayer::draw();

	// Set the vertex attributes to be position only
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	// Push a new matrix onto the stack
    kmGLPushMatrix();

	// Draw the debug data of the physics world
    m_pWorld->DrawDebugData();

	// Pop the new matrix off the stack
    kmGLPopMatrix();

	// Check for any GL errors
    CHECK_GL_ERROR_DEBUG();
#endif
}

MainLayer::~MainLayer()
{
	// Clean up all the children
	this->removeAllChildrenWithCleanup(true);

	// Release the instance of the player
	m_pPlayer->release();

	// Destroy the collision bodies
	m_pWorld->DestroyBody(m_pLevelBody);

	// Delete the physics world
	delete m_pWorld;
}

void MainLayer::onEnter()
{
	CCLayer::onEnter();

	// If objects aren't initialized
	if(m_pWorld == 0 || m_pLevelBody == 0 || m_pDebugRenderer == 0)
	{
		// Initialize all objects
		initObjects();
	}
}

void MainLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();

	// If the player isn't initialized
	if(m_pPlayer == 0)
	{
		// Initialize the player
		initPlayer();
	}
}

void MainLayer::onExit()
{
	CCLayer::onExit();
}

//Input callbacks
void MainLayer::ccTouchesBegan (CCSet *pTouches, CCEvent *pEvent)
{
	// Call the player's touch begin function
	m_pPlayer->TouchBegin();
}
void MainLayer::ccTouchesEnded (CCSet *pTouches, CCEvent *pEvent)
{
	// Call the player's touch end function
	m_pPlayer->TouchEnded();
}
