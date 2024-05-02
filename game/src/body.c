#include "body.h"

#include "integrator.h"
#include "world.h"

void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode) {
	if(body->bodyType != BT_Dynamic) {
		return;
	}

	switch(forceMode) {
		case FM_Force:
			body->force = Vector2Add(body->force, force);

			break;
		case FM_Impulse:
			// Applies a sudden change in momentum
			body->velocity = Vector2Scale(force, body->inverseMass);

			break;
		case FM_Velocity:
			body->velocity = force;

			break;
	}
}

void Step(ncBody* body, float timestep) {
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inverseMass);

	SemiImplicitEuler(body, timestep);

	// Damping
	float damping = 1.0f / (1.0f + body->damping * timestep);
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}