#pragma once

#include "body.h"

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

extern ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType);
extern void AddBody(ncBody* body);
extern void DestroyBody(ncBody* body);
extern void DestroyAllBodies();