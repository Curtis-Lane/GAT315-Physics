#pragma once

#include "raylib.h"

typedef enum ncBodyType ncBodyType;

typedef struct ncEditorData {
	bool bodyTypeEditMode;
	ncBodyType bodyTypeActive;
	float massMinValue;
	float massMaxValue;
	float gravityScaleValue;
	float dampingValue;
	float gravitationValue;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);