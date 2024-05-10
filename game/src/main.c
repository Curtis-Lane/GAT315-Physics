#include <stdlib.h>
#include <assert.h>

#include "raylib.h"
#include "raymath.h"

#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define BODIES_PER_CLICK 1

#define SCROLL_SENSITIVITY 0.2f

int main(void) {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	// Initialize world
	ncGravity = CreateVector2(0, -1);

	while(!WindowShouldClose()) {
		// Update
		float deltaTime = GetFrameTime();
		float fps = (float) GetFPS();

		Vector2 mousePosition = GetMousePosition();
		ncScreenZoom -= GetMouseWheelMove() * SCROLL_SENSITIVITY;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10.0f);
		UpdateEditor(mousePosition);

		if(!ncEditorIntersect && IsMouseButtonDown(0)) {
			// Explosions in a circle
			for(int i = 0; i < BODIES_PER_CLICK; i++) {
				ncBody* body = CreateBody(ConvertScreenToWorld(mousePosition), ncEditorData.massMinValue, ncEditorData.bodyTypeActive);
				//body->mass = GetRandomFloatValue(ncEditorData.massMinValue, ncEditorData.massMaxValue);
				body->damping = ncEditorData.dampingValue;
				body->gravityScale = ncEditorData.gravityScaleValue;
				body->color = ColorFromHSV(GetRandomFloatValue(0, 360), GetRandomFloat01(), GetRandomFloat01());
				//Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(1000, 2000));
				//ApplyForce(body, force, FM_Impulse);
			}
		}

		if(IsKeyPressed(KEY_R)) {
			DestroyAllBodies();
		}

		// Apply force
		ApplyGravitation(ncBodies, ncEditorData.gravitationValue);

		// Update bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			Step(body, deltaTime);
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Draw cursor circle
		//if(!ncEditorIntersect) {
		//	DrawCircle((int) mousePosition.x, (int) mousePosition.y, 15, YELLOW);
		//}

		// Render bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int) screen.x, (int) screen.y, ConvertWorldToPixel(body->mass), body->color);
			//DrawLine((int) body->prevPrevPosition.x, (int) body->prevPrevPosition.y, (int) body->prevPosition.x, (int) body->prevPosition.y, body->color);
			//DrawLine((int) body->prevPosition.x, (int) body->prevPosition.y, (int) body->position.x, (int) body->position.y, body->color);
		}

		// Draw the editor window and related utilities.
		DrawEditor(mousePosition);

		// Debug Stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", deltaTime), 10, 30, 20, LIME);

		EndDrawing();
	}
	CloseWindow();

	DestroyAllBodies();
	return 0;
}