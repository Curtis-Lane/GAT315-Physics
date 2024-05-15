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
	//
}

void ResolveContacts(ncContact* contacts) {
	//
}