#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "CommonHeader.h"
#include "Object.h"

//Defines for the various actions that the different obstacles will do (SHOULD LOAD FROM FILE)

//Speeds
#define FAST_SPEED			400.0f
#define MED_SPEED			225.0f
#define SLOW_SPEED			90.0f

//Sprite filenames
#define FAST_FILENAME		"Obstacles/FastObt.png"
#define MED_FILENAME		"Obstacles/MedObt.png"
#define SLOW_FILENAME		"Obstacles/SlowObt.png"

//Default names
#define NAME_FAST			"FastObt"
#define NAME_MED			"MedObt"
#define NAME_SLOW			"SlowObt"

//Tag for the action that will determine when the obstacle is done
#define TAG_OBSTACLE_DONE	1

enum ObstacleTypes{
	FAST_OBSTACLE = 1,
	MED_OBSTACLE,
	SLOW_OBSTACLE,
	OBSTACLE_COUNT};

// Basic obstacle in the game.
class Obstacle : public Object
{
	// Fields
	// --------------------
protected:

	// Collision body
	b2Body* m_pPhysicsBody;

	// Obstacle type
	int m_Type;

	// Obstacle speed
	float m_Speed;

	// Obstacle emitter 
	CCParticleSystem* m_pEmitter;
	
	// Methods
	// --------------------
public:
	// Reset obstacle with it current type
	bool reset();

	// Create a certain type
	static Obstacle* create(int type);
	// Init with a certain type
	bool initWithType(int type);

	// Overloaded update
	void update(float delta);

	//Getter & Setter
public:
	// Overlaod setSprite to make it do nothing because the sprite is now dependant on the type.
	void setSprite(CCSprite* sprite);
	
	// Getter for m_pPhysicsBody.  No setter because the body size is dependant on the type
	b2Body* getBody();

	// Returns the type of obstacle
	int getType();
	bool setType(float type);

	// Returns the width of the current sprite
	int getWidth();

	//Returns the height of the current sprite
	int getHeight();

	// Returns the current speed of the obstacle
	float getSpeed();

	// Other functions
protected:
	// init for a fast obstacle
	bool newFast();

	// init for a medium obstacle
	bool newMed();

	// init for a slow obstacle
	bool newSlow();

	// get's the distance between 2 points
	float distance(CCPoint start, CCPoint end);

	// reset for a fast obstacle
	bool resetFast();

	// reset for a medium obstacle
	bool resetMed();

	// reset for a slow obstacle
	bool resetSlow();

public:
	// Put the obstacle into "dormant"
	virtual bool dormant();

	// Deconstructor
	virtual ~Obstacle();

public:
};

#endif //__OBSTACLE_H__