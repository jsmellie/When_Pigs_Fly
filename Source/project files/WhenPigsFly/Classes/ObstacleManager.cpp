#include "ObstacleManager.h"
#include "Obstacle.h"
#include "MainLayer.h"
#include "GameScreen.h"


ObstacleManager* ObstacleManager::m_pInstance = NULL;

ObstacleManager* ObstacleManager::getInstance()
{
	if(m_pInstance != NULL)
	{
		return m_pInstance;
	}

	m_pInstance = new ObstacleManager();

	if(m_pInstance && m_pInstance->init())
	{
		m_pInstance->autorelease();

		m_pInstance->retain();

		return m_pInstance;
	}
	CC_SAFE_DELETE(m_pInstance);
	return NULL;
}

bool ObstacleManager::init()
{
	if(!GameScreen::getInstance() || !GameScreen::getInstance()->getMain())
	{
		return false;
	}

	m_pMainLayer = GameScreen::getInstance()->getMain();
	CC_SAFE_RETAIN(m_pMainLayer);

	this->scheduleUpdate();

	return true;
}

bool ObstacleManager::newObstacle()
{
	int random = (rand() % OBSTACLE_COUNT) + 1;

	return newObstacle(random);
}

bool ObstacleManager::newObstacle(int obstacleType)
{
	if(obstacleType <= 0 || obstacleType >= OBSTACLE_COUNT)
	{
		return newObstacle();
	}

	Obstacle* obstacle = NULL;

	if(m_ObstaclePool.size() > 0)
	{

		for(int i = 0; i < m_ObstaclePool.size(); ++i)
		{
			obstacle = m_ObstaclePool[i];

			if(obstacle != NULL)
			{
				if(obstacle->getType() == obstacleType)
				{
					m_ObstaclePool.erase(m_ObstaclePool.begin() + i);
					break;
				}
			}

			obstacle = NULL;
		}
	}

	if(obstacle == NULL)
	{
		do
		{
			obstacle = Obstacle::create(obstacleType);

			if(obstacle)
			{
				obstacle->retain();
				break;
			}
		}while(true);
	}
	obstacle->reset();

	m_ActiveObstacles.push_back(obstacle);
	this->addChild(obstacle);

	return true;
}

void ObstacleManager::update(float delta)
{
	Obstacle* obstacle = NULL;

	for(int i = 0; i < m_ActiveObstacles.size(); ++i)
	{
		obstacle = m_ActiveObstacles[i];

		if(obstacle == NULL)
		{
			m_ActiveObstacles.erase(m_ActiveObstacles.begin() + i);
			continue;

		}

		else if(obstacle->getActionByTag(TAG_OBSTACLE_DONE) == NULL)
		{
			m_ActiveObstacles.erase(m_ActiveObstacles.begin() + i);
			this->removeChild(obstacle);
			obstacle->dormant();
			m_ObstaclePool.push_back(obstacle);
		}
	}
}