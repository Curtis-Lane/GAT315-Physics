#include "editor.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

#include "render.h"
#include "world.h"
#include "spring.h"

#define EDITOR_DATA_FLOAT(data) TextFormat("%0.2f", data), &data

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;
ncEditorData_t ncEditorData;

Vector2 anchor01 = {950, 56};
Texture2D cursorTexture;

bool EditorBoxActive = true;

void InitEditor() {
	GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

	Image image = LoadImage("resources/reticle.png");
	cursorTexture = LoadTextureFromImage(image);
	UnloadImage(image);
	HideCursor();

	ncEditorData.bodyTypeEditMode = false;
	ncEditorData.bodyTypeActive = BT_Dynamic;
	ncEditorData.massValue = 1.5f;
	ncEditorData.gravityScaleValue = 0.0f;
	ncEditorData.dampingValue = 0.0f;
	ncEditorData.stiffnessValue = 20.0f;
	ncEditorData.restitutionValue = 0.5f;
	ncEditorData.gravitationValue = 2.0f;
	ncEditorData.gravityValue = 0.0f;
	ncEditorData.timestepValue = 50.0f;
	ncEditorData.simulationToggleActive = true;

	editorRect = (Rectangle){anchor01.x + 0, anchor01.y + 0, 304, 432};
}

static void ResetButton() {
	DestroyAllSprings();
	DestroyAllBodies();
}

void UpdateEditor(Vector2 position) {
	// Toggle show/hide editor box with key press
	if(IsKeyPressed(KEY_TAB)) {
		EditorBoxActive = !EditorBoxActive;
	}

	// Check if cursor position is intersecting the editor box
	ncEditorIntersect = EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position) {
	if(ncEditorData.bodyTypeEditMode) {
		GuiLock();
	}

	if(EditorBoxActive) {
		EditorBoxActive = !GuiWindowBox(editorRect, "Editor");
		GuiGroupBox((Rectangle) {anchor01.x + 16, anchor01.y + 48, 272, 200}, "Body Settings");
		GuiLabel((Rectangle) {anchor01.x + 128, anchor01.y + 56, 120, 24}, "BODY TYPE");
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 120, 120, 16}, "Mass", EDITOR_DATA_FLOAT(ncEditorData.massValue), 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 144, 120, 16}, "Gravity Scale", EDITOR_DATA_FLOAT(ncEditorData.gravityScaleValue), 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 168, 120, 16}, "Damping", EDITOR_DATA_FLOAT(ncEditorData.dampingValue), 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 192, 120, 16}, "Stiffness (k)", EDITOR_DATA_FLOAT(ncEditorData.stiffnessValue), 0, 50);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 216, 120, 16}, "Restitution", EDITOR_DATA_FLOAT(ncEditorData.restitutionValue), 0, 2.5f);
		GuiGroupBox((Rectangle) {anchor01.x + 16, anchor01.y + 264, 272, 104}, "World Settings");
		GuiSlider((Rectangle) {anchor01.x + 128, anchor01.y + 288, 120, 16}, "Gravity", EDITOR_DATA_FLOAT(ncEditorData.gravityValue), -25, 25);
		GuiSlider((Rectangle) {anchor01.x + 128, anchor01.y + 312, 120, 16}, "Gravitation Force", EDITOR_DATA_FLOAT(ncEditorData.gravitationValue), -10, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 128, anchor01.y + 336, 120, 16}, "Timestep", EDITOR_DATA_FLOAT(ncEditorData.timestepValue), 0, 120);
		if(GuiButton((Rectangle) {anchor01.x + 16, anchor01.y + 392, 120, 24}, "Reset")) ResetButton();
		GuiToggle((Rectangle) {anchor01.x + 176, anchor01.y + 392, 112, 24}, "Simulate", &ncEditorData.simulationToggleActive);
		if(GuiDropdownBox((Rectangle) {anchor01.x + 104, anchor01.y + 80, 120, 24}, "DYNAMIC;KINEMATIC;STATIC", (int*) &ncEditorData.bodyTypeActive, ncEditorData.bodyTypeEditMode)) ncEditorData.bodyTypeEditMode = !ncEditorData.bodyTypeEditMode;
	}

	DrawTexture(cursorTexture, (int) (position.x - cursorTexture.width * 0.5f), (int) (position.y - cursorTexture.height * 0.5f), WHITE);

	GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position) {
	for(ncBody* body = bodies; body != NULL; body = body->next) {
		Vector2 screen = ConvertWorldToScreen(body->position);
		if(CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f))) {
			return body;
		}
	}

	return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position) {
	Vector2 screen = ConvertWorldToScreen(body->position);
	DrawLine((int) screen.x, (int) screen.y, (int) position.x - cursorTexture.width / 2, (int) position.y - cursorTexture.height / 2, YELLOW);
}