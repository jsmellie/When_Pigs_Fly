#ifndef __CUSTOM_CONTACT_LISTENER_H__
#define __CUSTOM_CONTACT_LISTENER_H__

#include "CommonHeader.h"
#include <vector>

// Struct that will contain the collision data
struct CustomContact
{
	b2Fixture* fixtureA;
	b2Fixture* fixtureB;

	bool operator==(const CustomContact& other) const
	{
		return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
	}
};

// Class that will handle all collision contacts that happen
class CustomContactListener : public b2ContactListener
{
	// Fields
	// --------------------
public:
	//vector containing valid contact pairings
	std::vector<CustomContact> m_Contacts;

	
	// Methods
	// --------------------

	//Constructors and Decontructors
public:
	CustomContactListener();
	~CustomContactListener();

	// Function related to contact events
public:
	// Start of a contact
    virtual void BeginContact(b2Contact* contact);
	// End of a contact
    virtual void EndContact(b2Contact* contact);

	// Before the physics is solved
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//After the physics is solved
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

protected:
	// Analyse the names and determine if proper collision or not
	virtual bool AnalyseContact(const char* fixtureAName, const char* fixtureBName);

};

#endif //__CUSTOM_CONTACT_LISTENER_H__