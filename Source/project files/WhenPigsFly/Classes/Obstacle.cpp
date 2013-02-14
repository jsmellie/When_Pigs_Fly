#include "Obstacle.h"
#include "GameScreen.h"
#include "MainLayer.h"

bool Obstacle::reset()
{
	switch(m_Type)
	{
	case FAST_OBSTACLE:
		{
			resetFast();
			break;
		}
	case MED_OBSTACLE:
		{
			//resetMed();
			resetSlow();
			break;
		}
	case SLOW_OBSTACLE:
		{
			resetSlow();
			break;
		}
	default:
		{
			int bp = 0;
		}
	}

	return true;
}

Obstacle* Obstacle::create(int type)
{
	Obstacle* pobObstacle = new Obstacle();

	if(pobObstacle && pobObstacle->initWithType(type))
	{
		pobObstacle->autorelease();
		return pobObstacle;
	}
	CC_SAFE_DELETE(pobObstacle);
	return NULL;
}

bool Obstacle::initWithType(int type)
{
	if(!Object::init())
	{
		return false;
	}

	m_pPhysicsBody = NULL;
	m_Type = -1;
	m_Speed = -1;
	m_pEmitter = NULL;

	return setType(type);
}

void Obstacle::update(float delta)
{
	b2Vec2 currentPos(this->getPositionX() / PTM_RATIO, this->getPositionY() / PTM_RATIO);

	CCAssert(m_pSprite != NULL, "Sprite was null");

	float angle = -1 * CC_DEGREES_TO_RADIANS(m_pSprite->getRotation());

	b2Assert(m_pPhysicsBody != NULL, "Body was null");

	m_pPhysicsBody->SetTransform(currentPos, angle);
}

void Obstacle::setSprite(CCSprite* sprite) {}

b2Body* Obstacle::getBody()
{
	return m_pPhysicsBody;
}

int Obstacle::getType()
{
	return m_Type;
}

bool Obstacle::setType(float type)
{
	if(type > 0 && type < OBSTACLE_COUNT)
	{
		m_Type = type;

		switch(m_Type)
		{
		case FAST_OBSTACLE:
			{
				return newFast();
				break;
			}
		case SLOW_OBSTACLE:
			{
				return newSlow();
				break;
			}
		default:
			{
				return newMed();
				break;
			}
		}
	}
}

int Obstacle::getWidth()
{
	if(m_pSprite != NULL)
	{
		return m_pSprite->getContentSize().width;
	}

	return -1;
}

int Obstacle::getHeight()
{
	if(m_pSprite != NULL)
	{
		return m_pSprite->getContentSize().height;
	}

	return -1;
}

float Obstacle::getSpeed()
{
	return m_Speed;
}

bool Obstacle::newFast()
{
	//Get values of the screen size and it's origin
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	int screenWidth = visibleSize.width + origin.x;
	int screenHeight = visibleSize.height + origin.y;

	//Set speed to it's proper value
	m_Speed = FAST_SPEED;

	//Load the proper sprite
	m_pSprite = CCSprite::create(FAST_FILENAME);

	//Add m_pSprite as a child so that it's drawn each frame
	this->addChild(m_pSprite, 1);

	resetFast();

	return true;
}

bool Obstacle::newMed()
{
	return newSlow();
}

bool Obstacle::newSlow()
{
	//Get values of the screen size and it's origin
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	int screenWidth = visibleSize.width + origin.x;
	int screenHeight = visibleSize.height + origin.y;

	//Set speed to it's proper value
	m_Speed = SLOW_SPEED;

	//Load the proper sprite
	m_pSprite = CCSprite::create(SLOW_FILENAME);

	//Add m_pSprite as a child so that it's drawn each frame
	this->addChild(m_pSprite, 1);

	resetSlow();

	return true;
}

float Obstacle::distance(CCPoint start, CCPoint end)
{
	return sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
}

bool Obstacle::resetFast()
{
	//Get values of the screen size and it's origin
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	int screenWidth = visibleSize.width + origin.x;
	int screenHeight = visibleSize.height + origin.y;

	this->setName(NAME_FAST);

	//Pick a random Y start location
	int startY = (rand() % (screenHeight + 1));

	// Pick a random Y end location.  Using half screen height so that I can chose the opposite half
	// as startY
	int endY = (rand() % (screenHeight + 1));

	// CCPoint representing starting location
	CCPoint start = ccp(screenWidth+getWidth(), startY);
	this->setPosition(start);

	// CCPoint representing end location
	CCPoint end = ccp(0 - (getWidth() * 12), endY);

	// CCPoint representing the vector between
	CCPoint difference = ccp(end.x - start.x, end.y - start.y);

	// Distacne between start and end
	float dist = distance(start, end);

	float angle = tan(difference.y / difference.x);

	m_pSprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(angle));

	//Create the move to action for it going across the screen
	CCMoveTo* moveTo = CCMoveTo::create(dist / m_Speed, end);
	moveTo->setTag(TAG_OBSTACLE_DONE);

	if(m_pPhysicsBody != NULL)
	{
		m_pPhysicsBody->GetWorld()->DestroyBody(m_pPhysicsBody);
	}

	//Create the body definition for the collision body
	b2BodyDef obstacleDef;
	obstacleDef.type = b2_dynamicBody;
	obstacleDef.fixedRotation = true;
	obstacleDef.position.Set(this->getPositionX()/PTM_RATIO, this->getPositionY()/PTM_RATIO);
	obstacleDef.angle = (angle);
	obstacleDef.userData = this;

	// Initialize the physics body
	CCLayer* main = GameScreen::getInstance()->getMain();

	m_pPhysicsBody = GameScreen::getInstance()->getMain()->getWorld()->CreateBody(&obstacleDef);

	// Set up the shape of the box
	b2PolygonShape obstacleShape;
	obstacleShape.SetAsBox((getWidth()/2)/PTM_RATIO, (getHeight()/2)/PTM_RATIO);

	//set up the physics properties for the fixture
	b2FixtureDef obstacleFixture;
	obstacleFixture.shape = &obstacleShape;
	obstacleFixture.density = 0;
	obstacleFixture.friction = 0;
	obstacleFixture.restitution = 0;

	//Creat ethe ficture with the definition
	m_pPhysicsBody->CreateFixture(&obstacleFixture);

	//Initialize the emitter
	if(m_pEmitter == NULL)
	{
		m_pEmitter = CCParticleSmoke::createWithTotalParticles(200, getHeight() / 2);

		m_pEmitter->setGravity(ccp(0, 100));

		m_pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(SMOKE_FILENAME));
		m_pEmitter->setLife(0.5f);
		m_pEmitter->setLifeVar(0.1f);
		m_pEmitter->setEmissionRate(m_pEmitter->getTotalParticles() / (m_pEmitter->getLife() * 2));

		m_pEmitter->setPosition(getWidth(), getHeight() - m_pEmitter->getStartSize()/2);

		CCParticleSystem* fireSystem = CCParticleFire::createWithTotalParticles(200);
		fireSystem->setGravity(ccp(0,0));
		fireSystem->setPosVar(ccp(1,1));
		fireSystem->setLife(0.1f);
		fireSystem->setLifeVar(0.05f);
		fireSystem->setEmissionRate(fireSystem->getTotalParticles() / (fireSystem->getLife() * 2));
		fireSystem->setStartSize(m_pEmitter->getStartSize()/2);
		fireSystem->setPosition(ccp(0, 0 - fireSystem->getStartSize()));

		m_pEmitter->addChild(fireSystem, 1);

		m_pSprite->addChild(m_pEmitter, -1);
	}

	// Run the action
	this->runAction(moveTo);

	if(m_isUpdating == false)
	{
		this->scheduleUpdate();
		m_isUpdating = true;
	}

	return true;
}

bool Obstacle::resetMed()
{
	return true;
}


bool Obstacle::resetSlow()
{
	//Get values of the screen size and it's origin
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	int screenWidth = visibleSize.width + origin.x;
	int screenHeight = visibleSize.height + origin.y;

	this->setName(NAME_SLOW);

	int startY = rand()  % (screenHeight/2);

	int endY = startY + screenHeight/4;

	int highestY = endY + 60;

	CCPoint start = ccp(-getWidth(), startY);
	CCPoint end = ccp(-getWidth(), endY);

	CCPoint highest = ccp(screenWidth/2, highestY);

	CCPoint startToHighest = ccp(highest.x - start.x, highest.y - start.y);
	CCPoint highestToEnd = ccp(end.x - highest.x, end.y - highest.y);

	float dist = distance(start, highest);

	CCRotateBy* rotPerSecond = CCRotateBy::create(1, 270);
	CCRepeatForever* rotAction = CCRepeatForever::create(rotPerSecond);
	m_pSprite->runAction(rotAction);

	CCMoveTo* moveToMiddle = CCMoveTo::create(dist / m_Speed, highest);
	CCEaseSineOut* easeToMiddle = CCEaseSineOut::create(moveToMiddle);

	dist = distance(highest, end);

	CCMoveTo* moveToEnd = CCMoveTo::create(dist / m_Speed, end);
	CCEaseSineIn* easeToEnd = CCEaseSineIn::create(moveToEnd);

	//CCArray* actionArray = CCArray::create(easeToMiddle, CCDelayTime::create(0.5f), easeToEnd);

	CCSequence* movementSequence = (CCSequence*)CCSequence::create(easeToMiddle, CCDelayTime::create(0.5f), easeToEnd, NULL);
	//CCSequence* movementSequence = CCSequence::createWithTwoActions(easeToMiddle, easeToEnd);
	movementSequence->setTag(TAG_OBSTACLE_DONE);

	this->runAction(movementSequence);
	
	if(m_pPhysicsBody != NULL)
	{
		m_pPhysicsBody->GetWorld()->DestroyBody(m_pPhysicsBody);
	}

	//Create the body definition for the collision body
	b2BodyDef obstacleDef;
	obstacleDef.type = b2_dynamicBody;
	obstacleDef.fixedRotation = true;
	obstacleDef.position.Set(this->getPositionX()/PTM_RATIO, this->getPositionY()/PTM_RATIO);
	obstacleDef.angle = (-1 * CC_DEGREES_TO_RADIANS(m_pSprite->getRotation()));
	obstacleDef.userData = this;

	// Initialize the physics body
	m_pPhysicsBody = GameScreen::getInstance()->getMain()->getWorld()->CreateBody(&obstacleDef);

	// Set up the shape of the box
	b2PolygonShape obstacleShape;
	obstacleShape.SetAsBox((getWidth()/2)/PTM_RATIO, (getHeight()/2)/PTM_RATIO);

	//set up the physics properties for the fixture
	b2FixtureDef obstacleFixture;
	obstacleFixture.shape = &obstacleShape;
	obstacleFixture.density = 0;
	obstacleFixture.friction = 0;
	obstacleFixture.restitution = 0;

	//Creat ethe ficture with the definition
	m_pPhysicsBody->CreateFixture(&obstacleFixture);

	if(m_isUpdating == false)
	{
		this->scheduleUpdate();
		m_isUpdating = true;
	}

	return true;
}

bool Obstacle::dormant()
{
	if(m_pPhysicsBody != NULL)
	{
		m_pPhysicsBody->GetWorld()->DestroyBody(m_pPhysicsBody);
		m_pPhysicsBody = NULL;
	}

	if(m_pEmitter != NULL)
	{
		m_pSprite->removeChild(m_pEmitter, true);
		m_pEmitter = NULL;
	}

	if(m_isUpdating == true)
	{
		this->unscheduleUpdate();
		m_isUpdating = false;
	}

	return true;
}

Obstacle::~Obstacle()
{
}
