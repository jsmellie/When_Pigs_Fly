#ifndef __PARALLAX_OBJECT_H__
#define __PARALLAX_OBJECT_H__

#include "CommonHeader.h"
#include "Object.h"

// Parallax relevent defines
#define PARALLAX_CONSTANT	20.0f

class ParallaxObject : public Object
{
	//Fields
	//---------------------------
protected:
	// Looping sprite for parallax
	CCSprite* m_pLoopingSprite;

	// Speed that the sprite will move each second
	float m_Speed;
	
	// Methods
	//---------------------------
public:
	// Basic create function
	static ParallaxObject* create();
	// Base Init function
	bool init();

	// Create with a filename
	static ParallaxObject* create(const char* fileName);
	// Init with a const filename
	bool initWithFileName(const char* fileName);

	// Create with a filename
	static ParallaxObject* create(const char* fileName, int depth);
	// Init with a const filename
	bool initWithFileName(const char* fileName, int depth);

	// Update function
	virtual void update(float delta);

	// Getter for m_pLoopingSprite
	virtual CCSprite* getLoopingSprite();

	//Overload the setter for m_pSprite
	virtual void setSprite(const char* fileName);

	//Overload the setter for zOrder
	virtual void setZOrder(int nZOrder);

	// Deconstructor
	virtual ~ParallaxObject();

protected:
	// Initial init, no matter what the input
	bool basicSetup();
};

#endif