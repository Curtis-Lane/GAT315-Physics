#pragma once

#include "body.h"

extern ncBody* ncBodies;
extern int ncBodyCount;

extern ncBody* CreateBody();
extern void DestroyBody(ncBody* body);
extern void DestroyAllBodies();