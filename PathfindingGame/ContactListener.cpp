#include "ContactListener.h"
#include "RigidBodyComponent.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	RigidBodyComponent* r1 = (RigidBodyComponent*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	RigidBodyComponent* r2 = (RigidBodyComponent*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (r2 && r1)
	{
		if (contact->GetFixtureA()->IsSensor())
			r1->OnTriggerEnter(r2, contact->GetFixtureB());
		else
			r1->OnCollisionEnter(r2, contact->GetFixtureB());

		if (contact->GetFixtureB()->IsSensor())
			r2->OnTriggerEnter(r1, contact->GetFixtureA());
		else
			r2->OnCollisionEnter(r1, contact->GetFixtureA());
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	RigidBodyComponent* r1 = (RigidBodyComponent*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	RigidBodyComponent* r2 = (RigidBodyComponent*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (r1 && r2)
	{
		if (contact->GetFixtureA()->IsSensor())
			r1->OnTriggerLeave(r2, contact->GetFixtureB());
		else
			r1->OnCollisionLeave(r2, contact->GetFixtureB());

		if (contact->GetFixtureB()->IsSensor())
			r2->OnTriggerLeave(r1, contact->GetFixtureA());
		else
			r2->OnCollisionLeave(r1, contact->GetFixtureA());
	}
}
