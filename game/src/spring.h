#pragma once

#include "body.h"

typedef struct ncSpring {
	ncBody* bodyOne;
	ncBody* bodyTwo;
	float restLength;
	float k; // Stiffness

	struct ncSpring* next;
	struct ncSpring* prev;
} ncSpring;

extern ncSpring* ncSprings;

extern ncSpring* CreateSpring(ncBody* bodyOne, ncBody* bodyTwo, float restLength, float k);
extern void AddSpring(ncSpring* spring);
extern void DestroySpring(ncSpring* spring);
extern void DestroyAllSprings();

extern void ApplySpringForce(ncSpring* springs);
extern void ApplySpringForcePosition(Vector2 position, ncBody* body, float restLength, float k, float damping);