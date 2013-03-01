#include "CustomContactListener.h"
#include <algorithm>
#include "Player.h"
#include "Obstacle.h"
#include "ObstacleManager.h"
#include "GameScreen.h"
#include "MainLayer.h"
#include "Player.h"

CustomContactListener::CustomContactListener() : m_Contacts() {}

CustomContactListener::~CustomContactListener() {}

void CustomContactListener::BeginContact(b2Contact* contact)
{
	CustomContact customContact = { contact->GetFixtureA(), contact->GetFixtureB()};

	const char* fixtureAName = 0;
	const char* fixtureBName = 0;

	void* fixtureAUserData = customContact.fixtureA->GetBody()->GetUserData();

	if(fixtureAUserData != 0)
	{
		Object* fixtureAObj = (Object*)fixtureAUserData;

		if(fixtureAObj->getName() != 0)
		{
			fixtureAName = fixtureAObj->getName();
		}
	}

	void* fixtureBUserData = customContact.fixtureB->GetBody()->GetUserData();

	if(fixtureBUserData != 0)
	{
		Object* fixtureBObj = (Object*)fixtureBUserData;

		if(fixtureBObj->getName() != 0)
		{
			fixtureBName = fixtureBObj->getName();
		}
	}

	if(fixtureAName != 0 && fixtureBName != 0)
	{
		if(AnalyseContact(fixtureAName, fixtureBName))
		{
			m_Contacts.push_back(customContact);

			GameScreen::getInstance()->getMain()->getPlayer()->crash();
		}
	}
}

void CustomContactListener::EndContact(b2Contact* contact)
{
	CustomContact customContact = { contact->GetFixtureA(), contact->GetFixtureB()};

	std::vector<CustomContact>::iterator pos;

	pos = std::find(m_Contacts.begin(), m_Contacts.end(), customContact);

	if(pos != m_Contacts.end())
	{
		m_Contacts.erase(pos);
	}
}

void CustomContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}

void CustomContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

bool CustomContactListener::AnalyseContact(const char* fixtureAName, const char* fixtureBName)
{
	if(fixtureAName == NAME_PLAYER || fixtureBName == NAME_PLAYER)
	{
		if(fixtureAName == NAME_FAST || fixtureBName == NAME_FAST)
			return true;

		else if(fixtureAName == NAME_SLOW || fixtureBName == NAME_SLOW)
			return true;
		
		else if(fixtureAName == NAME_MED || fixtureBName == NAME_MED)
			return true;
	}

	return false;
}