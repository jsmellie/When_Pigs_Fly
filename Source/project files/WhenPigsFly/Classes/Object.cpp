#include "Object.h"

CCSprite* Object::getSprite()
{
	return m_pSprite;
}

void Object::setSprite(CCSprite* sprite)
{
	if(sprite != NULL)
	{
		if(m_pSprite !=NULL)
		{
			this->removeChild(m_pSprite);
			m_pSprite->release();
		}

		m_pSprite = sprite;
		this->addChild(m_pSprite);
		m_pSprite->retain();
	}
}

const char* Object::getName()
{
	return m_Name;
}

void Object::setName(const char* name)
{
	m_Name = name;
}

bool Object::init()
{
	if(!CCDirector::sharedDirector())
	{
		return false;
	}

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	this->setPosition(ccp(origin.x, origin.y));

	m_pSprite = NULL;
	m_Name = NULL;

	//Add the update func to the schedule
	this->scheduleUpdate();
	m_isUpdating = true;

	return true;
}

Object::~Object()
{
	if(m_pSprite != NULL)
		m_pSprite->release();
}