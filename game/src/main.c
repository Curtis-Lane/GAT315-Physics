#include <stdlib.h>
#include <assert.h>

#include "raylib.h"
#include "raymath.h"

#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//#define MAX_BODIES 100

int main(void) {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Engine");
	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		// Update
		float deltaTime = GetFrameTime();
		float fps = (float) GetFPS();

		Vector2 mousePosition = GetMousePosition();
		if(IsMouseButtonDown(0)) {
			ncBody* body = CreateBody();
			body->position = mousePosition;
			body->mass = GetRandomFloatValue(1.0f, 5.0f);
			//ApplyForce(body, CreateVector2(GetRandomFloatValue(-50.0f, 50.0f), GetRandomFloatValue(-50.0f, 50.0f)));
		}

		// Apply force
		{
			ncBody* body = ncBodies;
			while(body != NULL) {
				ApplyForce(body, CreateVector2(0, -50));
				
				body = body->next;
			}
		}

		// Update bodies
		{
			ncBody* body = ncBodies;
			while(body != NULL) {
				ExplicitEuler(body, deltaTime);
				ClearForce(body);

				body = body->next;
			}
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Debug Stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", deltaTime), 10, 30, 20, LIME);

		DrawCircle((int) mousePosition.x, (int) mousePosition.y, 15, YELLOW);

		// Render bodies
		{
			ncBody* body = ncBodies;
			while(body != NULL) {
				DrawCircle((int) body->position.x, (int) body->position.y, body->mass, RED);

				body = body->next;
			}
		}

		EndDrawing();
	}
	CloseWindow();

	DestroyAllBodies();
	return 0;
}