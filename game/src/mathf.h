#pragma once

#include <stdlib.h>

#include "raylib.h"

inline float GetRandomFloat01() {
	return rand() / (float) RAND_MAX;
}

inline float GetRandomFloatValue(float min, float max) {
	if(min > max) {
		float temp = max;
		max = min;
		min = temp;
	}

	return ((max - min) * GetRandomFloat01()) + min;
}

inline Vector2 CreateVector2(float x, float y) {
	return (Vector2) {x, y};
}

inline Vector2 GetVector2FromAngle(float angle) {
	// Create a vector using cosine/sine of the angle
	return CreateVector2(cosf(angle), sinf(angle));
}
