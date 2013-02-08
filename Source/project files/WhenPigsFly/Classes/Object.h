#ifndef OBJECT_H
#define OBJECT_H

#include "cocos2d.h"

USING_NS_CC;

//Base abstract class for all objects that will be in my game
class Object : public CCNode
{
	//Fields
	//---------------------------
protected:
	//Basic sprite
	CCSprite* m_pSprite;

	//Bool to say weather update is scheduled (Can't find a way to check if it is with isScheduled)
	bool m_isUpdating;

	// Name of the current object, names can be unique but shouldn't be contead on
	const char* m_Name;
	

	// Methods
	//---------------------------

	// Getters and setters
public:
	// Returns the current sprite
	virtual CCSprite* getSprite();
	// Sets the current sprite
	virtual void setSprite(CCSprite* sprite);

	// Returns the object's name
	virtual const char* getName();
	// Sets the object's name
	virtual void setName(const char* name);


	//Game loop and creation
public:
	//Init function
	bool init();

	//Base deconstructor
	virtual ~Object();

	//Virtual update
	virtual void update(float delta) = 0;


};

#endif