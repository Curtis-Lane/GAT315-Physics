#pragma once

#include "raylib.h"

typedef struct ncContact ncContact;
typedef struct ncBody ncBody;

void CreateContacts(ncBody* bodies, ncContact** contacts);
ncContact* GenerateContact(ncBody* bodyOne, ncBody* bodyTwo);

void SeparateContacts(ncContact* contacts);
void ResolveContacts(ncContact* contacts);