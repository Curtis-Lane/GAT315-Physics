#include "force.h"

#include "body.h"

void ApplyGravitation(ncBody* bodies, float strength) {
	for(ncBody* bodyOne = bodies; bodyOne != NULL; bodyOne = bodyOne->next) {
		for(ncBody* bodyTwo = bodies; bodyTwo != NULL; bodyTwo = bodyTwo->next) {
			if(bodyOne == bodyTwo) {
				continue;
			}

			Vector2 direction = Vector2Subtract(bodyOne->position, bodyTwo->position);
			float distance = Vector2Length(direction);

			distance = fmaxf(1.0f, distance);
			float force = (bodyOne->mass * bodyTwo->mass / (distance * distance)) * strength;

			direction = Vector2Scale(Vector2Normalize(direction), force);
			
			ApplyForce(bodyOne, Vector2Negate(direction), FM_Force);
			ApplyForce(bodyTwo, direction, FM_Force);
		}
	}
}