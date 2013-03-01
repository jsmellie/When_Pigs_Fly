#ifndef __OBSTACLE_MANAGER_H__
#define __OBSTACLE_MANAGER_H__

#include "CommonHeader.h"

class Obstacle;
class MainLayer;

// Class that will manage all obstacles and pool all currently unused obstacles
class ObstacleManager : public CCNode
{
	// Fields
	// --------------------
protected:
	// Vector of obstacles 
	std::vector<Obstacle*> m_ObstaclePool;

	// Static instance of the obstacle manager
	static ObstacleManager* m_pInstance;

	// Vector containing all currently active obstacles
	std::vector<Obstacle*> m_ActiveObstacles;

	// Reference to the main layer of the game screen
	MainLayer* m_pMainLayer;

	// Methods
	// --------------------
public:
	// Returns the static ObstacleMnager instance
	static ObstacleManager* getInstance();

	// Basic init
	bool init();

	// Activates a random type of obstacle.  Return true if successfull
	bool newObstacle();

	// Activates a obstacle of type passed.  If the type passed was invalid, creates a random obstacle.  Return true if successfull
	bool newObstacle(int type);

	// Checks to see if any of the obstacles are off the edge of the screen
	void update(float delta);

	// Deconstructor
	virtual ~ObstacleManager();

	// There has been a colission with the player
	void collisionWithPlayer(Obstacle* obstacle);

};

#endif