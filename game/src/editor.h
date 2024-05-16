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
	float stiffnessValue;
	float restitutionValue;
	float gravityValue;
	float gravitationValue;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);