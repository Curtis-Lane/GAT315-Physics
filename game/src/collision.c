#include "collision.h"

#include "stdlib.h"
#include "assert.h"
#include "string.h"

#include "contact.h"
#include "body.h"
#include "mathf.h"

bool Intersects(ncBody* bodyOne, ncBody* bodyTwo) {
	// if distance < radius
	float distance = Vector2Distance(bodyOne->position, bodyTwo->position);
	float radius = bodyOne->mass + bodyTwo->mass;

	return distance <= radius;
}

void CreateContacts(ncBody* bodies, ncContact** contacts) {
	for(ncBody* bodyOne = bodies; bodyOne != NULL; bodyOne = bodyOne->next) {
		for(ncBody* bodyTwo = bodyOne->next; bodyTwo != NULL; bodyTwo = bodyTwo->next) {
			if(bodyOne == bodyTwo) {
				continue;
			}

			if(bodyOne->bodyType != BT_Dynamic && bodyTwo->bodyType != BT_Dynamic) {
				continue;
			}

			if(Intersects(bodyOne, bodyTwo)) {
				ncContact* contact = GenerateContact(bodyOne, bodyTwo);
				AddContact(contact, contacts);
			}
		}
	}
}

ncContact* GenerateContact(ncBody* bodyOne, ncBody* bodyTwo) {
	// Allocate memory for a new contact
	ncContact* contact = (ncContact*) malloc(sizeof(ncContact));
	// Ensure that memory was allocated successfully
	assert(contact != NULL);
	// Overwrite the memory with zeros so that we don't end up using the random junk data that happened to exist in that memory
	memset(contact, 0, sizeof(ncContact));

	// Set the contact's two bodies
	contact->bodyOne = bodyOne;
	contact->bodyTwo = bodyTwo;

	// Calculate the direction vector of the contact
	Vector2 direction = Vector2Subtract(bodyOne->position, bodyTwo->position);
	// Calculate the distance between the bodies
	float distance = Vector2Length(direction);
	// Ensure that the direction can never equal zero, so that we avoid a potential divide by zero error
	if(distance == 0) {
		direction = CreateVector2(GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f));
	}

	// Get the combined radii of the two bodies
	// In this case, that would be the combined mass of the bodies, since that is what we are currently using for the radius
	float radius = (bodyOne->mass + bodyTwo->mass);

	// Calculate the depth of the penetration
	contact->depth = radius - distance;
	// Normalize the direction vector to create the contact's normal vector
	contact->normal = Vector2Normalize(direction);
	// Calculate the combined restitution of the two bodies
	contact->restitution = (bodyOne->restitution + bodyTwo->restitution) * 0.5f;

	// Return the newly created contact
	return contact;
}

void SeparateContacts(ncContact* contacts) {
	// Iterate over every contact that was provided
	for(ncContact* contact = contacts; contact != NULL; contact = contact->next) {
		// Calculate the combined inverse mass of both of the contact's bodies
		float totalInvMass = contact->bodyOne->inverseMass + contact->bodyTwo->inverseMass;

		// Calculate the direction that would most directly separate the two bodies
		// This is done by scaling the normal vector of the contact by the depth of the contact divided by the total inverse mass
		Vector2 seperation = Vector2Scale(contact->normal, contact->depth / totalInvMass);

		// Move each body so that they are no longer overlapping
		// The second body is sent in the opposite direction of the separate vector so that the two bodies are sent in opposing directions
		contact->bodyOne->position = Vector2Add(contact->bodyOne->position, Vector2Scale(seperation, contact->bodyOne->inverseMass));
		contact->bodyTwo->position = Vector2Add(contact->bodyTwo->position, Vector2Negate(Vector2Scale(seperation, contact->bodyTwo->inverseMass)));
	}
}

void ResolveContacts(ncContact* contacts) {
	// Iterate over every contact that was provided
	for(ncContact* contact = contacts; contact != NULL; contact = contact->next) {
		// Calculate the relative velocity between the two bodies
		Vector2 relativeVelocity = Vector2Subtract(contact->bodyOne->velocity, contact->bodyTwo->velocity);
		// Calculate the normal velocity of the collision
		float normalVelocity = Vector2DotProduct(relativeVelocity, contact->normal);
		// If the bodies are moving in the same direction, skip further calculations for current collision
		if(normalVelocity > 0.0f) {
			continue;
		}

		// Calculate the combined inverse mass of both of the contact's bodies
		float totalInvMass = contact->bodyOne->inverseMass + contact->bodyTwo->inverseMass;
		// Calculate the impulse magnitude using the restitution coefficient and the relative velocity
		float impulseMagnitude = (((1.0f + contact->restitution) * -1) * normalVelocity / totalInvMass);
		// Convert that impluse magnitude into a vector by scaling the contact's normal by the impulse magnitude
		Vector2 impulseVector = Vector2Scale(contact->normal, impulseMagnitude);

		// Apply the impulse force to both bodies
		// The second body has the force inverted so that the two bodies move in opposite directions
		ApplyForce(contact->bodyOne, impulseVector, FM_Impulse);
		ApplyForce(contact->bodyTwo, Vector2Negate(impulseVector), FM_Impulse);
	}
}