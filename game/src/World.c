#include "world.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;

ncBody* CreateBody() {
	ncBody* body = (ncBody*) malloc(sizeof(ncBody));
	assert(body != NULL);

	memset(body, 0, sizeof(ncBody));

	body->prev = NULL;
	body->next = ncBodies;

	if(ncBodyCount != 0) {
		ncBodies[0].prev = body;
	}
	ncBodyCount += 1;

	ncBodies = body;

	return body;
}

void DestroyBody(ncBody* body) {
	assert(body != NULL);

	if(body->prev != NULL) {
		(body->prev)->next = body->next;
	}
	if(body->next != NULL) {
		(body->next)->prev = body->prev;
	}

	if(body == ncBodies) {
		ncBodies = body->next;
	}
	ncBodyCount -= 1;

	free(body);
}

void DestroyAllBodies() {
	ncBody* body = ncBodies;
	ncBody* currentBody = NULL;
	for(int i = 0; i < ncBodyCount; i++) {
		currentBody = body->next;
		DestroyBody(body);
		body = currentBody;
	}
}