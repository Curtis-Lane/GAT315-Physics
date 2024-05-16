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
	ncContact* contact = (ncContact*) malloc(sizeof(ncContact));
	assert(contact != NULL);
	memset(contact, 0, sizeof(ncContact));

	contact->bodyOne = bodyOne;
	contact->bodyTwo = bodyTwo;

	Vector2 direction = Vector2Subtract(bodyOne->position, bodyTwo->position);
	float distance = Vector2Length(direction);
	if(distance == 0) {
		direction = CreateVector2(GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f));
	}

	float radius = (bodyOne->mass + bodyTwo->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (bodyOne->restitution + bodyTwo->restitution) * 0.5f;

	return contact;
}

void SeparateContacts(ncContact* contacts) {
	for(ncContact* contact = contacts; contact != NULL; contact = contact->next) {
		float totalInvMass = contact->bodyOne->inverseMass + contact->bodyTwo->inverseMass;
		Vector2 seperation = Vector2Scale(contact->normal, contact->depth / totalInvMass);
		contact->bodyOne->position = Vector2Add(contact->bodyOne->position, Vector2Scale(seperation, contact->bodyOne->inverseMass));
		contact->bodyTwo->position = Vector2Add(contact->bodyTwo->position, Vector2Negate(Vector2Scale(seperation, contact->bodyTwo->inverseMass)));
	}
}

void ResolveContacts(ncContact* contacts) {
	for(ncContact* contact = contacts; contact != NULL; contact = contact->next) {
		Vector2 relativeVelocity = Vector2Subtract(contact->bodyOne->velocity, contact->bodyTwo->velocity);
		float normalVelocity = Vector2DotProduct(relativeVelocity, contact->normal);
		if(normalVelocity > 0.0f) {
			continue;
		}

		float totalInvMass = contact->bodyOne->inverseMass + contact->bodyTwo->inverseMass;
		float impulseMagnitude = (((1.0f + contact->restitution) * -1) * normalVelocity / totalInvMass);
		Vector2 impulseVector = Vector2Scale(contact->normal, impulseMagnitude);

		ApplyForce(contact->bodyOne, impulseVector, FM_Impulse);
		ApplyForce(contact->bodyTwo, Vector2Negate(impulseVector), FM_Impulse);
	}
}