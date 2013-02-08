#include "ParallaxObject.h"

ParallaxObject* ParallaxObject::create()
{
	//Create the new pointer
    ParallaxObject *pobParallax = new ParallaxObject();

	//If the new pointer isn't null and init() is successfull
    if (pobParallax && pobParallax->init())
    {
		//Set it to auto release
        pobParallax->autorelease();
		//Return the new pointer
        return pobParallax;
    }
	//Safely delete the pointer
    CC_SAFE_DELETE(pobParallax);
	// Return null
    return NULL;
}

bool ParallaxObject::init()
{
	// If the base class's init() isn't successfull, return false
	if(!Object::init())
	{
		return false;
	}
	
	// Set variables to default value
	basicSetup();

	return true;
}

ParallaxObject* ParallaxObject::create(const char* fileName)
{
	//Create the new pointer
    ParallaxObject *pobParallax = new ParallaxObject();

	//If the new pointer isn't null and initWithFileName(filename) is successfull
    if (pobParallax && pobParallax->initWithFileName(fileName))
    {
		//Set it to auto release
        pobParallax->autorelease();
		//Return the new pointer
        return pobParallax;
    }
	//Safely delete the pointer
    CC_SAFE_DELETE(pobParallax);
	// Return null
    return NULL;
}

bool ParallaxObject::initWithFileName(const char* fileName)
{
	// If the base class's init() isn't successfull, return false
	if(!Object::init())
	{
		return false;
	}
	
	// Set variables to default value
	basicSetup();

	// Initialize the sprites with the proper filename
	this->setSprite(fileName);

	return true;
}

ParallaxObject* ParallaxObject::create(const char* fileName, int zOrder)
{
	//Create the new pointer
    ParallaxObject *pobParallax = new ParallaxObject();

	//If the new pointer isn't null and initWithFileName(filename, zOrder) is successfull
    if (pobParallax && pobParallax->initWithFileName(fileName, zOrder))
    {
		//Set it to auto release
        pobParallax->autorelease();
		//Return the new pointer
        return pobParallax;
    }
	//Safely delete the pointer
    CC_SAFE_DELETE(pobParallax);
	// Return null
    return NULL;
}

bool ParallaxObject::initWithFileName(const char* fileName, int zOrder)
{
	// If the base class's init() isn't successfull, return false
	if(!Object::init())
	{
		return false;
	}
	
	// Set variables to default value
	basicSetup();

	// Initialize zOrder with value passed in
	this->setZOrder(zOrder);

	// Initialize the sprites with the proper filename
	this->setSprite(fileName);

	return true;
}

void ParallaxObject::update(float delta)
{
	// float to hold m_pLoopingSprite's current X pos
	float loopingPosX = m_pLoopingSprite->getPositionX();

	// decrement it by speed
	loopingPosX -= m_Speed;

	// float to hold m_pSprite's current X pos
	float spritePosX = m_pSprite->getPositionX();

	// decrement it by speed
	spritePosX -= m_Speed;

	// If m_pLoopingSprite is now off the edge of the screen
	if(loopingPosX < -m_pLoopingSprite->getContentSize().width)
	{
		// Set it to be to the right of m_pSprite
		loopingPosX = m_pSprite->getPositionX() + (m_pLoopingSprite->getContentSize().width - m_Speed);
	}
	
	// If m_pSprite is now off the edge of the screen
	if(spritePosX < -m_pSprite->getContentSize().width)
	{
		// Set it to be to the right of m_pLoopingSprite
		spritePosX = m_pLoopingSprite->getPositionX() + (m_pSprite->getContentSize().width - m_Speed);
	}

	// Set the sprites' positions to the proper values
	m_pLoopingSprite->setPositionX(loopingPosX);
	m_pSprite->setPositionX(spritePosX);	
}

CCSprite* ParallaxObject::getLoopingSprite()
{
	//Just returns m_pLoopingSprite
	return m_pLoopingSprite;
}

void ParallaxObject::setSprite(const char* fileName)
{
	// If m_pLoopingSprite isn't null
	if(m_pLoopingSprite != NULL)
	{
		// Remove it as a child
		this->removeChild(m_pLoopingSprite);
		// Release it
		m_pLoopingSprite->release();
	}
	
	// If m_pLoopingSprite isn't null
	if(m_pSprite != NULL)
	{
		// Remove it as a child
		this->removeChild(m_pSprite);
		// Release it
		m_pSprite->release();
	}

	// Make m_pLoopingSprite equal a new sprite with the filename passed in
	m_pLoopingSprite = CCSprite::create(fileName);
	// Set it's X pos to the position of this object plus the width of m_pLoopingSprite
	m_pLoopingSprite->setPositionX(m_pLoopingSprite->getContentSize().width);
	// Retain it so that we can hold onto a copy of it
	m_pLoopingSprite->retain();
	// Add it as a child
	this->addChild(m_pLoopingSprite);
	
	// Make m_pSprite equal a new sprite with the filename passed in
	m_pSprite = CCSprite::create(fileName);
	// Set it's X pos to the position of this object
	m_pSprite->setPositionX(0);
	// Retain it so that we can hold onto a copy of it
	m_pSprite->retain();
	// Add it as a child
	this->addChild(m_pSprite);
}

void ParallaxObject::setZOrder(int zOrder)
{
	// Get the origin and the visible size of the application
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// if the zOrder is bigger then -1
	if(zOrder > -1)
	{
		// Make it -1
		zOrder = -1;
	}

	// Call the base classes setZOrder
	Object::setZOrder(zOrder);

	// Find the absolute value of zOrder
	float depth = abs(zOrder);
	
	// Calculate the speed based on the depth
	m_Speed = PARALLAX_CONSTANT / depth;
}

ParallaxObject::~ParallaxObject()
{
	CC_SAFE_RELEASE(m_pLoopingSprite);
}

bool ParallaxObject::basicSetup()
{
	// Get the origin and the visible size of the application
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// Set the default positions
	this->setPositionX(visibleSize.width/2 + origin.x);
	this->setPositionY(visibleSize.height/2 + origin.y);

	// Set the default zOrder to -1
	this->setZOrder(-1);

	//Set m_pLoopingSprite to null
	m_pLoopingSprite = NULL;

	return true;
}