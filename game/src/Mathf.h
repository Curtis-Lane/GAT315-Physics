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