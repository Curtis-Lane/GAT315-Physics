#pragma once

//#include "raymath.h"

#include "body.h"

//typedef struct ncBody ncBody;

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

extern ncBody* CreateBody();
extern void DestroyBody(ncBody* body);
extern void DestroyAllBodies();