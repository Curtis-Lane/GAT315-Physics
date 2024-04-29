#include "world.h"

#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody() {
	Body* body = (Body*) malloc(sizeof(Body));
	assert(body != NULL);

	body->prev = NULL;
	body->next = bodies;

	if(bodyCount != 0) {
		bodies[0].prev = body;
	}
	bodyCount += 1;

	bodies = body;

	return body;
}

void DestroyBody(Body* body) {
	assert(body != NULL);

	if(body->prev != NULL) {
		(body->prev)->next = body->next;
	}
	if(body->next != NULL) {
		(body->next)->prev = body->prev;
	}

	if(body == bodies) {
		bodies = body->next;
	}
	bodyCount -= 1;

	free(body);
}

void DestroyAllBodies() {
	Body* body = bodies;
	Body* currentBody = NULL;
	for(int i = 0; i < bodyCount; i++) {
		currentBody = body->next;
		DestroyBody(body);
		body = currentBody;
	}
}