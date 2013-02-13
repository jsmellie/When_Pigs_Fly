#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"
#include "CommonHeader.h"

//Player sprite information (Should make it load from a file)
#define PLAYER_WIDTH			104
#define PLAYER_HEIGHT			41
#define PLAYER_SPRITE			"Player.png"
#define PLAYER_FRAME_COUNT		4
#define PLAYER_DELAY			0.05f
#define PLAYER_PADDING			2

//Player physics information (Should make it load from a file)
#define PLAYER_DESITY			2.0f
#define PLAYER_FRICTION			0.2f
#define PLAYER_REST				0.0f
#define PLAYER_MAXSPEED			20.0f

//Player rotation numbers.  All in degrees
#define PLAYER_LIFT_ROT			7.0f
#define PLAYER_LIFT_SPEED		10.0f

#define PLAYER_IDLE_ROT			13.0f
#define PLAYER_IDLE_SPEED		10.0f

//Action tags various actions that will be used.  Good for stopping specific actions
#define TAG_PLAYER_LIFT			3
#define TAG_PLAYER_IDLE			4
#define TAG_PLAYER_ANI			1
#define TAG_PLAYER_CRASH		2

// Player's name
#define NAME_PLAYER				"Player"

// Crashing variables
#define PLAYER_CRASH_SPEED		200.0f

class Player : public Object
{
	//Fields
	//---------------------------
protected:
	//Is the player alive
	bool m_isAlive;

	//Is the user pressing the input
	bool m_isInputPressed;

	//Body of the player
	b2Body* m_pPhysicsBody;

	// Joint making it so that the player can only move vertically before he gets hit
	b2Joint* m_jVertJoint;

	// Acceleration on click
	b2Vec2 m_vThrust;

	// Particle emitter for the smoke.
	CCParticleSystem* m_pEmitter;

	// Bool to represent if physics body needs to be removed
	bool m_DeleteBody;

	// Methods
	//---------------------------

	// Getters and setters
public:

	//Overrite sprite setter so that it no longer does anything so that you can't
	//accidentally override player's sprite
	virtual void setSprite(CCSprite* sprite);

	//Overrite name setter so that it no longer does anything.  We don't want it to be changed
	virtual void setName(const char* name);

	// Returns true if the physics body needs to be deleted
	virtual bool shouldDeleteBody();

	// Returns the players physics body
	virtual b2Body* getBody();

	// Gameloop and creation
public:
	//Create the "create()" function
	CREATE_FUNC(Player);
	//Init function
	bool init();

	//Deconstructor
	virtual ~Player();

	//Update
	virtual void update(float delta);

	// Input and callbacks
public:
	//Input functions
	virtual void TouchBegin();
	virtual void TouchEnded();

	//Helper functions
public:
	//Is called when it's time for the player to crash
	virtual void crash();

	// Is called when the physics body can be destroyed.
	// I have to use a bool to queu that it's ready to be destroyed because the world might be in the mittle of updating.
	virtual void destroyBody();

protected:
	// Creates a fire explosion on the players current position
	virtual void createFireExplosion();

	// Creates a smoke explosion on the players current position
	virtual void createSmokeExplosion();

	// Createsa fire particle system and a smoke particle system to simulate the helicopter on fire and crashing
	virtual void createBurningEffect();

	// Creates the action sequence that show the player crashing
	virtual void addCrashAction();
};

#endif