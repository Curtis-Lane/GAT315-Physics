#pragma once

#include "raymath.h"

typedef struct Body {
	// Force -> acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	struct Body* next;
	struct Body* prev;
} Body;