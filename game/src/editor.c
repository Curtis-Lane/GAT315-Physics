#include "editor.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

#include "render.h"
#include "body.h"

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
	ncEditorData.massMinValue = 1.5f;
	ncEditorData.massMaxValue = 3.0f;
	ncEditorData.gravityScaleValue = 0.0f;
	ncEditorData.dampingValue = 0.0f;
	ncEditorData.stiffnessValue = 20.0f;
	ncEditorData.gravitationValue = 2.0f;
	ncEditorData.gravityValue = 0.0f;

	editorRect = (Rectangle){anchor01.x + 0, anchor01.y + 0, 304, 616};
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
		EditorBoxActive = !GuiWindowBox((Rectangle) {anchor01.x + 0, anchor01.y + 0, 304, 616}, "Editor");
		GuiGroupBox((Rectangle) {anchor01.x + 16, anchor01.y + 48, 272, 192}, "Body Settings");
		GuiLabel((Rectangle) {anchor01.x + 128, anchor01.y + 56, 120, 24}, "BODY TYPE");
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 120, 120, 16}, "Mass Min", TextFormat("%0.2f", ncEditorData.massMinValue), &ncEditorData.massMinValue, 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 144, 120, 16}, "Mass Max", TextFormat("%0.2f", ncEditorData.massMaxValue), &ncEditorData.massMaxValue, 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 168, 120, 16}, "Gravity Scale", TextFormat("%0.2f", ncEditorData.gravityScaleValue), &ncEditorData.gravityScaleValue, 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 192, 120, 16}, "Damping", TextFormat("%0.2f", ncEditorData.dampingValue), & ncEditorData.dampingValue, 0, 10);
		GuiSliderBar((Rectangle) {anchor01.x + 104, anchor01.y + 216, 120, 16}, "Stiffness (k)", TextFormat("%0.2f", ncEditorData.stiffnessValue), & ncEditorData.stiffnessValue, 0, 50);
		GuiGroupBox((Rectangle) {anchor01.x + 16, anchor01.y + 256, 272, 152}, "World Settings");
		GuiSlider((Rectangle) {anchor01.x + 128, anchor01.y + 296, 120, 16}, "Gravitation Force", TextFormat("%0.2f", ncEditorData.gravitationValue), &ncEditorData.gravitationValue, 0, 10);
		GuiSlider((Rectangle) {anchor01.x + 128, anchor01.y + 272, 120, 16}, "Gravity", TextFormat("%0.2f", ncEditorData.gravityValue), &ncEditorData.gravityValue, -25, 25);
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