#pragma once

#include "raylib.h"
#include "raymath.h"

typedef enum ncBodyType {
	BT_Dynamic,
	BT_Kinematic,
	BT_Static
} ncBodyType;

typedef enum ncForceMode {
	FM_Force,
	FM_Impulse,
	FM_Velocity
} ncForceMode;

typedef struct ncBody {
	ncBodyType bodyType;

	// Force -> acceleration -> velocity -> position
	Vector2 position;
	// These were here purely for trail rendering and will likely be removed.
	//Vector2 prevPosition;
	//Vector2 prevPrevPosition;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inverseMass; // static = 0
	float gravityScale;
	float damping;

	float restitution;

	Color color;

	struct ncBody* next;
	struct ncBody* prev;
} ncBody;

void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode);

inline void ClearForce(ncBody* body) {
	body->force = Vector2Zero();
}

void Step(ncBody* body, float timestep);
