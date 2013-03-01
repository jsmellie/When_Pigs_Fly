#include "Player.h"
#include "GameScreen.h"
#include "MainLayer.h"
#include "GameOverScreen.h"

void Player::setSprite(CCSprite* sprite) {}

void Player::setName(const char* name) {}

bool Player::shouldDeleteBody()
{
	return m_DeleteBody;
}

b2Body* Player::getBody()
{
	return m_pPhysicsBody;
}

Player::~Player()
{
	this->removeAllChildrenWithCleanup(true);

	m_jVertJoint = 0;

	destroyBody();

	m_pEmitter = 0;
}

bool Player::init()
{
	//Previous init
	if(!Object::init())
	{
		return false;
	}

	// Default values
	m_DeleteBody = false;
	m_Name = NAME_PLAYER;
	m_vThrust = b2Vec2(0, 40.0f);
	m_isInputPressed = false;
	m_pPhysicsBody = 0;

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	//Set the starting position
	this->setPosition(-PLAYER_WIDTH, visibleSize.height/2 + origin.y);

	//Load in the sprite sheet
	CCSpriteBatchNode* spriteSheet = CCSpriteBatchNode::create(PLAYER_FILENAME);

	//Set up the sprite itself
	m_pSprite = CCSprite::createWithTexture(spriteSheet->getTexture(), CCRectMake(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));

	m_pSprite->setRotation(PLAYER_IDLE_ROT * 2);
	m_pSprite->retain();

	//Add the sprite to the sprite sheet
	spriteSheet->addChild(m_pSprite, 2);

	//Creation of players animation
	CCAnimation* playerAni = CCAnimation::create();
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

	//Make spritesheet a player child
	this->addChild(spriteSheet, 2);

	// Initialize the smoke trail
	m_pEmitter = CCParticleSmoke::createWithTotalParticles(200, PLAYER_HEIGHT / 12);

	// If the initialization went properly
	if(m_pEmitter != 0)
	{
		// Set custom smoke attributes
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

	float introTime = 1.8f;

	// Create the basic intro action
	CCMoveTo* pMoveTo = CCMoveTo::create(introTime, ccp(visibleSize.width/4 + origin.x, visibleSize.height/2 + origin.y));
	CCEaseOut* moveEase = CCEaseOut::create(pMoveTo, 3);

	CCCallFunc* pCallFunc = CCCallFunc::create(this, callfunc_selector(Player::initPhysics));

	CCSequence* pSeq1 = CCSequence::createWithTwoActions(moveEase, CCDelayTime::create(0.2f));

	MainLayer* main = GameScreen::getInstance()->getMain();

	CCSequence* pSeq2 = CCSequence::createWithTwoActions(pCallFunc, CCCallFunc::create(main, callfunc_selector(MainLayer::activateObstacles)));

	CCSequence* pIntro = CCSequence::createWithTwoActions(pSeq1, pSeq2);
	pIntro->setTag(TAG_PLAYER_INTRO);

	this->runAction(pIntro);

	float rotTime = 0.8f;

	CCRotateTo* pRotate = CCRotateTo::create(rotTime, PLAYER_IDLE_ROT);
	CCEaseSineOut* rotateEase = CCEaseSineOut::create(pRotate);

	CCSequence* pRotSeq = CCSequence::createWithTwoActions(CCDelayTime::create(introTime - rotTime), rotateEase);

	m_pSprite->runAction(pRotSeq);

	return true;
}

void Player::update(float delta)
{
	if(m_pPhysicsBody != 0)
	{
		if(shouldDeleteBody())
		{
			destroyBody();
		}
		else
		{
			CCAction* temp = this->getActionByTag(TAG_PLAYER_INTRO);
			if(this->getActionByTag(TAG_PLAYER_INTRO))
			{
				int bp = 0;
			}
			//Visible size of the screen
			CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

			// If the player is moving too fast, put on damping
			float32 speed = m_pPhysicsBody->GetLinearVelocity().Length();

			// If the player is going too fast, put damping on the physics body to slow the acceleration
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

			// Get the physics body's position
			b2Vec2 bodyPos = m_pPhysicsBody->GetPosition();

			// Set the position of the player to the physics body's position
			this->setPosition(bodyPos.x * PTM_RATIO, bodyPos.y * PTM_RATIO);
		}
	}
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

void Player::crash()
{
	createFireExplosion();
	createSmokeExplosion();

	m_DeleteBody = true;

	createBurningEffect();

	addCrashAction();
}

void Player::destroyBody()
{
	if(m_pPhysicsBody != 0)
	{
		m_pPhysicsBody->GetWorld()->DestroyBody(m_pPhysicsBody);
		m_pPhysicsBody = 0;
	}

	m_DeleteBody = false;
}

void Player::initPhysics()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	b2World* world = GameScreen::getInstance()->getMain()->getWorld();

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
}

void Player::createFireExplosion()
{
	CCParticleSystem* fireExplosion = CCParticleExplosion::createWithTotalParticles(200);

	fireExplosion->setPosition(ccp(0,0));
	fireExplosion->setLife(0.5f);
	fireExplosion->setLifeVar(0.1f);

	//fireExplosion->setTexture(CCTextureCache::sharedTextureCache()->addImage(FIRE_FILENAME));

	ccColor4F startColor;// color of particles
	startColor.r = 1.0f;
	startColor.g = 0.0f;
	startColor.b = 0.0f;
	startColor.a = 1.0f;

	ccColor4F endColor;
	endColor.r = 1.0f;
	endColor.g = 1.0f;
	endColor.b = 1.0f;
	endColor.a = 0.0f;

	ccColor4F starVarColor;
	starVarColor.r = -0.1f;
	starVarColor.g = 0.7f;
	starVarColor.b = 0.2f;
	starVarColor.a = -0.7f;

	ccColor4F endVarColor;
	endVarColor.r = -0.1f;
	endVarColor.g = -0.1f;
	endVarColor.b = 0.05f;
	endVarColor.a = 0.0f;


	fireExplosion->setStartColor(startColor);
	fireExplosion->setStartColorVar(starVarColor);
	fireExplosion->setEndColor(endColor);
	fireExplosion->setEndColorVar(endVarColor);

	this->addChild(fireExplosion, 10);
}

void Player::createSmokeExplosion()
{
	CCParticleSystem* smokeExplosion = CCParticleExplosion::createWithTotalParticles(200);

	smokeExplosion->setTexture(CCTextureCache::sharedTextureCache()->addImage(SMOKE_FILENAME));

	smokeExplosion->setGravity(ccp(0, 100));

	smokeExplosion->setPosition(ccp(0,0));
	smokeExplosion->setLife(1.0f);
	smokeExplosion->setLifeVar(0.1f);
	
	ccColor4F startColor;// color of particles
	startColor.r = 0.6f;
	startColor.g = 0.6f;
	startColor.b = 0.6f;
	startColor.a = 1.0f;
	
	ccColor4F endColor;
	endColor.r = 0.0f;
	endColor.g = 0.0f;
	endColor.b = 0.0f;
	endColor.a = 0.0f;

	smokeExplosion->setStartColor(startColor);
	smokeExplosion->setStartColorVar(endColor);
	smokeExplosion->setEndColor(endColor);
	smokeExplosion->setEndColorVar(endColor);

	this->addChild(smokeExplosion, 8);
}

void Player::createBurningEffect()
{

}

void Player::addCrashAction()
{
	//Get the distance between the player and the bottum of the screen plus the player's width
	float distanceY = this->getPositionY() + m_pSprite->getContentSize().width;

	// Determine the percentage of hieght the player is from the bottum of the screen
	float yPercentage = distanceY / (CCDirector::sharedDirector()->getVisibleSize().height + m_pSprite->getContentSize().width);

	// With the Y percentage, calculate how far back in X the player should move
	float distanceX = this->getPositionX() * yPercentage;

	// Calculate the time it will take
	float time = distanceY / PLAYER_CRASH_SPEED;

	// Creation of the movement action
	CCMoveBy* moveAction = CCMoveBy::create(distanceY / PLAYER_CRASH_SPEED, ccp(-distanceX, -distanceY));

	// Turn the basic verticle movement into a ease in so that it looks like it's accelerating
	CCEaseIn* moveEase = CCEaseIn::create(moveAction, 1.5f);

	// Create the function call action
	CCCallFunc* pFuncCall = CCCallFunc::create(this, callfunc_selector(Player::gameOver));

	// Combine the movement with the function call action
	CCSequence* pCrashAction = CCSequence::createWithTwoActions(moveEase, pFuncCall);
	pCrashAction->setTag(TAG_PLAYER_CRASH);

	// Run the action
	this->runAction(pCrashAction);
}

void Player::gameOver()
{
	// Get the current instance of the game screen
	GameScreen* game = GameScreen::getInstance();

	// Get a reference to the background layer
	CCLayer* backLayer = game->getBackground();
	backLayer->retain();

	// Release the background layer from the game screen
	game->releaseBackground();

	// Create a game over screen and set it's back layer to the background layer
	GameOverScreen* gameOver = GameOverScreen::createWithBackLayer(GameScreen::getInstance()->getBackground());

	// replace the game screen with the game over menu
	CCDirector::sharedDirector()->replaceScene(gameOver);

	// release the current reference of the background layer
	backLayer->release();
}