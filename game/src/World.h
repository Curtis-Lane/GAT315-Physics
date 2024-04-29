#pragma once

#include "body.h"

extern Body* bodies;
extern int bodyCount;

extern Body* CreateBody();
extern void DestroyBody(Body* body);
extern void DestroyAllBodies();