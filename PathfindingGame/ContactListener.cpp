#include "ContactListener.h"
#include "RigidBodyComponent.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	b2Manifold* m = contact->GetManifold();
	RigidBodyComponent* r1 = (RigidBodyComponent*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	RigidBodyComponent* r2 = (RigidBodyComponent*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (r1)
		r1->OnCollisionEnter(r2, m);
	if (r2)
		r2->OnCollisionEnter(r1, m);
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2Manifold* m = contact->GetManifold();
	RigidBodyComponent* r1 = (RigidBodyComponent*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	RigidBodyComponent* r2 = (RigidBodyComponent*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (r1)
		r1->OnCollisionLeave(r2, m);
	if (r2)
		r2->OnCollisionLeave(r1, m);
}
