#include "spring.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring* ncSprings = NULL;

ncSpring* CreateSpring(ncBody* bodyOne, ncBody* bodyTwo, float restLength, float k) {
	ncSpring* spring = (ncSpring*) malloc(sizeof(ncSpring));
	assert(spring != NULL);
	memset(spring, 0, sizeof(ncSpring));

	spring->bodyOne = bodyOne;
	spring->bodyTwo = bodyTwo;
	spring->restLength = restLength;
	spring->k = k;

	return spring;
}

void AddSpring(ncSpring* spring) {
	assert(spring != NULL);

	spring->prev = NULL;
	spring->next = ncSprings;

	if(ncSprings != NULL) {
		ncSprings->prev = spring;
	}

	ncSprings = spring;
}

void DestroySpring(ncSpring* spring) {
	assert(spring != NULL);

	if(spring->prev != NULL) {
		(spring->prev)->next = spring->next;
	}
	if(spring->next != NULL) {
		(spring->next)->prev = spring->prev;
	}

	if(spring == ncSprings) {
		ncSprings = spring->next;
	}

	free(spring);
}

void DestroyAllSprings() {
	ncSpring* spring = ncSprings;
	ncSpring* currentSpring = NULL;
	while(spring != NULL) {
		currentSpring = spring->next;
		DestroySpring(spring);
		spring = currentSpring;
	}
}

void ApplySpringForce(ncSpring* springs) {
	for(ncSpring* spring = springs; spring != NULL; spring = spring->next) {
		Vector2 direction = Vector2Subtract(spring->bodyOne->position, spring->bodyTwo->position);
		if(direction.x == 0 && direction.y == 0) {
			continue;
		}

		float length = Vector2Length(direction);
		float x = length - spring->restLength;
		float force = x * spring->k * -1;

		Vector2 nDirection = Vector2Normalize(direction);

		ApplyForce(spring->bodyOne, Vector2Scale(nDirection, force), FM_Force);
		ApplyForce(spring->bodyTwo, Vector2Scale(Vector2Negate(nDirection), force), FM_Force);
	}
}

void ApplySpringForcePosition(Vector2 position, ncBody* body, float restLength, float k, float damping) {
	// Check if the body pointer is null; if so, exit the function
	if(body == NULL) {
		return;
	}

	// Calculate the direction vector from the body's position to the given position
	Vector2 direction = Vector2Subtract(position, body->position);

	// If the direction vector is zero (i.e., positions are the same), exit the function
	if(direction.x == 0 && direction.y == 0) {
		return;
	}

	// Calculate the length of the direction vector (distance between the two positions)
	float length = Vector2Length(direction);

	// Calculate the displacement from the rest length
	float x = length - restLength;

	// Apply Hooke's Law (f = -kx) to determine the spring force
	float force = x * k * -1;

	// Normalize the direction vector
	Vector2 nDirection = Vector2Normalize(direction);

	// Calculate the damping force (opposing force due to velocity)
	float dampingForce = damping * Vector2DotProduct(body->velocity, nDirection);

	// Calculate the total force by combining spring force and damping force
	float totalForce = force + dampingForce;

	// Apply the total force to the body in the direction of the normalized vector
	ApplyForce(body, Vector2Scale(nDirection, -totalForce), FM_Force);
}