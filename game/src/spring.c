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

	AddSpring(spring);

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