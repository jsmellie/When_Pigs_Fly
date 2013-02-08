#include "Player.h"
#include "GameScreen.h"
#include "MainLayer.h"

Player::~Player()
{
}

bool Player::init()
{
	if(!Object::init())
	{
		return false;
	}

	m_Name = NAME_PLAYER;

	b2World* world = GameScreen::getInstance()->getMain()->getWorld();

	m_vThrust = b2Vec2(0, 40.0f);

	m_isInputPressed = false;
	
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	//Load in the sprite sheet
	CCSpriteBatchNode* spriteSheet = CCSpriteBatchNode::create(PLAYER_SPRITE);

	//Set up the sprite itself
	m_pSprite = CCSprite::createWithTexture(spriteSheet->getTexture(), CCRectMake(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));

	m_pSprite->setRotation(PLAYER_IDLE_ROT);

	//Add the sprite to the sprite sheet
	spriteSheet->addChild(m_pSprite, 2);

	//Creation of players animation
	CCAnimation* playerAni = CCAnimation::animation();
	playerAni->setDelayPerUnit(PLAYER_DELAY);

	//Set up every frame of the animation
	for(int i = 0; i < PLAYER_FRAME_COUNT; ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(spriteSheet->getTexture(), CCRectMake(i * (PLAYER_WIDTH + PLAYER_PADDING), 0, PLAYER_WIDTH, PLAYER_HEIGHT));
		playerAni->addSpriteFrame(frame);
	}

	//Create the CCAnimate action
	CCAnimate* playerAniAction = CCAnimate::create(playerAni);
	CCRepeatForever* repPlayerAni = CCRepeatForever::create(playerAniAction);

	repPlayerAni->setTag(TAG_PLAYER_ANI);

	//make the sprite animate
	m_pSprite->runAction(repPlayerAni);

	//Create the physics body definition
	b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.fixedRotation = true;

	//Starting position for the player
	CCPoint pos = ccp(visibleSize.width/4 + origin.x, visibleSize.height/2 + origin.y);

	//Setting the physics body starting position
	playerBodyDef.position.Set(pos.x/PTM_RATIO, pos.y/PTM_RATIO);
	//Setting the physics body rotation value in radians
	playerBodyDef.angle = (-1 * CC_DEGREES_TO_RADIANS(m_pSprite->getRotation()));
	//Setting this instance to the userdata so that in a collision callback I have referrence to it
	playerBodyDef.userData = this;

	m_pPhysicsBody = world->CreateBody(&playerBodyDef);

	b2PolygonShape playerShape;
	playerShape.SetAsBox((PLAYER_WIDTH/2)/PTM_RATIO, (PLAYER_HEIGHT/2)/PTM_RATIO);

	b2FixtureDef playerShapeDef;
	playerShapeDef.shape = &playerShape;
	playerShapeDef.density = PLAYER_DESITY;
	playerShapeDef.friction = PLAYER_FRICTION;
	playerShapeDef.restitution = PLAYER_REST;

	m_pPhysicsBody->CreateFixture(&playerShapeDef);

	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(0.0f, 1.0f);

	jointDef.collideConnected = true;
	jointDef.Initialize(m_pPhysicsBody,
						GameScreen::getInstance()->getMain()->getLevelBody(),
						m_pPhysicsBody->GetWorldCenter(),
						worldAxis);

	m_jVertJoint = world->CreateJoint(&jointDef);

	//Make spritesheet a player child
	this->addChild(spriteSheet, 2);

	//m_pEmitter = NULL;
	m_pEmitter = CCParticleSmoke::createWithTotalParticles(200, PLAYER_HEIGHT / 12);

	if(m_pEmitter != NULL)
	{
		m_pEmitter->setTexture(CCTextureCache::sharedTextureCache()->addImage(SMOKE_FILENAME));
		m_pEmitter->setGravity(ccp(-400, 100));

		m_pEmitter->setLife(0.8f);
		m_pEmitter->setLifeVar(0.1f);
		m_pEmitter->setEmissionRate(m_pEmitter->getTotalParticles() / (m_pEmitter->getLife() * 2));

		m_pEmitter->setEndSize(m_pEmitter->getStartSize() * 6);

		ccColor4F endColor = ccColor4F();

		endColor.r = endColor.b = endColor.g = 1;
		endColor.a = 0;

		m_pEmitter->setEndColor(endColor);
		m_pEmitter->setEndColorVar(m_pEmitter->getStartColorVar());

		m_pEmitter->setPosition(m_pEmitter->getPositionX() - (PLAYER_HEIGHT/10), m_pEmitter->getPositionY() - (PLAYER_HEIGHT/3));

		this->addChild(m_pEmitter, -1);
	}

	return true;
}

void Player::setSprite(CCSprite* sprite) {}

void Player::setName(const char* name) {}

void Player::update(float delta)
{
	//Visible size of the screen
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	// If the player is moving too fast, put on damping
	float32 speed = m_pPhysicsBody->GetLinearVelocity().Length();

	if(speed > PLAYER_MAXSPEED)
	{
		m_pPhysicsBody->SetLinearDamping(0.5f);
	}
	else
	{
		m_pPhysicsBody->SetLinearDamping(0.0f);
	}

	//If the user pressing input
	if(m_isInputPressed)
	{
		//Apply a verticle force to lift the helicopter
		m_pPhysicsBody->ApplyLinearImpulse(m_vThrust, m_pPhysicsBody->GetPosition());
	}

	//Convert m_pSprite's rotation to radians
	float spriteRot = -1 * CC_DEGREES_TO_RADIANS(m_pSprite->getRotation());

	//If the angle for the physics body and the sprite are different
	if(m_pPhysicsBody->GetAngle() != spriteRot)
	{
		//Set the physics rotation to the sprite rotation
		m_pPhysicsBody->SetTransform(m_pPhysicsBody->GetPosition(), spriteRot);
	}

	b2Vec2 bodyPos = m_pPhysicsBody->GetPosition();

	this->setPosition(bodyPos.x * PTM_RATIO, bodyPos.y * PTM_RATIO);
}

// At the beginning of touch input
void Player::TouchBegin()
{
	//Set the update bool for input to true
	m_isInputPressed = true;
	
	//Calculate the difference between the current angle and the lift angle
	float difInRotation = abs(PLAYER_LIFT_ROT - m_pSprite->getRotation());
	
	//If there is a difference
	if(difInRotation != 0)
	{
		//Stop the idle rotation action
		m_pSprite->stopActionByTag(TAG_PLAYER_IDLE);
		
		//Create the new CCRotateTO action with the appropriate 
		CCRotateTo* rotateTo = CCRotateTo::create(difInRotation / PLAYER_LIFT_SPEED, PLAYER_LIFT_ROT);
		//Set the actions tag to be the proper value
		rotateTo->setTag(TAG_PLAYER_LIFT);
		
		//Run the rotate action
		m_pSprite->runAction(rotateTo);
	}
}

// At the end of touch input
void Player::TouchEnded()
{
	//Set the update bool for input to false
	m_isInputPressed = false;

	//Calculate the difference between the current angle and the idle angle
	float difInRotation = abs(PLAYER_IDLE_ROT - m_pSprite->getRotation());

	//If there is a difference
	if(difInRotation != 0)
	{
		//Stop the lift rotation action
		m_pSprite->stopActionByTag(TAG_PLAYER_LIFT);

		//Create the new CCRotateTO action with the appropriate 
		CCRotateTo* rotateTo = CCRotateTo::create(difInRotation / PLAYER_IDLE_SPEED, PLAYER_IDLE_ROT);
		//Set the actions tag to be the proper value
		rotateTo->setTag(TAG_PLAYER_IDLE);

		//Run the rotate action
		m_pSprite->runAction(rotateTo);
	}
}