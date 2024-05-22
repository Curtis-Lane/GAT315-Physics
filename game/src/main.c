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
#include "spring.h"
#include "contact.h"
#include "collision.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define BODIES_PER_CLICK 1

#define SCROLL_SENSITIVITY 0.2f

int main(void) {
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	ncContact* contacts = NULL;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	float timeAccumulator = 0.0f;

	// Initialize world
	ncGravity = CreateVector2(0, -1);

	while(!WindowShouldClose()) {
		// Update
		float deltaTime = GetFrameTime();
		float fps = (float) GetFPS();
		timeAccumulator += deltaTime;
		float fixedTimestep = 1.0f / ncEditorData.timestepValue;

		ncGravity = CreateVector2(0.0f, ncEditorData.gravityValue);

		Vector2 mousePosition = GetMousePosition();
		ncScreenZoom -= GetMouseWheelMove() * SCROLL_SENSITIVITY;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10.0f);
		UpdateEditor(mousePosition);

		selectedBody = GetBodyIntersect(ncBodies, mousePosition);
		if(selectedBody != NULL) {
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int) screen.x, (int) screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) * 1.1f, YELLOW);
		}

		if(!ncEditorIntersect) {
			// Create body
			if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT))) {
				for(int i = 0; i < BODIES_PER_CLICK; i++) {
					ncBody* body = CreateBody(ConvertScreenToWorld(mousePosition), ncEditorData.massValue, ncEditorData.bodyTypeActive);
					body->damping = ncEditorData.dampingValue;
					body->gravityScale = ncEditorData.gravityScaleValue;
					body->color = WHITE; //ColorFromHSV(GetRandomFloatValue(0, 360), GetRandomFloat01(), max(GetRandomFloat01(), 0.5f));
					body->restitution = ncEditorData.restitutionValue;

					AddBody(body);
				}
			}

			if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody != NULL) {
				connectBody = selectedBody;
			}

			if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody != NULL) {
				DrawLineBodyToPosition(connectBody, mousePosition);
			}

			if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody != NULL) {
				if(selectedBody != NULL && selectedBody != connectBody) {
					ncSpring* spring = CreateSpring(selectedBody, connectBody, Vector2Distance(selectedBody->position, connectBody->position), ncEditorData.stiffnessValue);
					AddSpring(spring);
				}
			}

			if(IsKeyDown(KEY_LEFT_SHIFT)) {
				if(connectBody != NULL) {
					Vector2 world = ConvertScreenToWorld(mousePosition);

					if(connectBody->bodyType != BT_Dynamic) {
						connectBody->position = world;
					} else {
						ApplySpringForcePosition(world, connectBody, 0, 20, 5);
					}
				}
			}

			if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
				selectedBody = NULL;
				connectBody = NULL;
			}
		}

		if(IsKeyPressed(KEY_R) || ncEditorData.isResetPressed) {
			DestroyAllSprings();
			DestroyAllBodies();
		}

		if(IsKeyPressed(KEY_T)) {
			DestroyAllSprings();
		}

		if(IsKeyPressed(KEY_P)) {
			ncEditorData.simulationToggleActive = !ncEditorData.simulationToggleActive;
		}

		while(timeAccumulator >= fixedTimestep) {
			timeAccumulator -= fixedTimestep;

			if(!ncEditorData.simulationToggleActive) {
				continue;
			}

			// Apply forces
			ApplyGravitation(ncBodies, ncEditorData.gravitationValue);
			ApplySpringForce(ncSprings);

			// Update bodies
			for(ncBody* body = ncBodies; body != NULL; body = body->next) {
				Step(body, fixedTimestep);
			}

			DestroyAllContacts(&contacts);

			// Collision
			CreateContacts(ncBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Render springs
		for(ncSpring* spring = ncSprings; spring != NULL; spring = spring->next) {
			Vector2 screenOne = ConvertWorldToScreen(spring->bodyOne->position);
			Vector2 screenTwo = ConvertWorldToScreen(spring->bodyTwo->position);
			DrawLine((int) screenOne.x, (int) screenOne.y, (int) screenTwo.x, (int) screenTwo.y, YELLOW);
		}

		// Render bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int) screen.x, (int) screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}

		// Render contacts
		for(ncContact* contact = contacts; contact != NULL; contact = contact->next) {
			Vector2 screen = ConvertWorldToScreen(contact->bodyOne->position);
			DrawCircle((int) screen.x, (int) screen.y, ConvertWorldToPixel(contact->bodyOne->mass * 0.5f), RED);
			screen = ConvertWorldToScreen(contact->bodyTwo->position);
			DrawCircle((int) screen.x, (int) screen.y, ConvertWorldToPixel(contact->bodyTwo->mass * 0.5f), RED);
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