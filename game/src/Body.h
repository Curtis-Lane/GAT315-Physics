#pragma once

#include "raymath.h"

typedef enum ncBodyType {
	Static,
	Kinematic,
	Dynamic
} ncBodyType;

typedef struct ncBody {
	ncBodyType bodyType;

	// Force -> acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	float mass;
	float inverseMass; // static = 0

	struct ncBody* next;
	struct ncBody* prev;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force) {
	body->force = Vector2Add(body->force, force);
}

inline void ClearForce(ncBody* body) {
	body->force = Vector2Zero();
}