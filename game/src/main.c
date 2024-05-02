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

int main(void) {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Physics Engine");
	SetTargetFPS(60);

	// Initialize world
	ncGravity = CreateVector2(0, 30);

	while(!WindowShouldClose()) {
		// Update
		float deltaTime = GetFrameTime();
		float fps = (float) GetFPS();

		Vector2 mousePosition = GetMousePosition();
		if(IsMouseButtonDown(0)) {
			ncBody* body = CreateBody();
			body->position = mousePosition;
			body->mass = GetRandomFloatValue(1.0f, 5.0f);
			body->inverseMass = 1 / body->mass;
			body->bodyType = BT_Dynamic;
			body->damping = 0.5f;
			body->gravityScale = 6.0f;
			ApplyForce(body, CreateVector2(GetRandomFloatValue(-150.0f, 150.0f), GetRandomFloatValue(-150.0f, 150.0f)), FM_Velocity);
		}

		// Apply force
		//for(ncBody* body = ncBodies; body != NULL; body = body->next) {
		//	ApplyForce(body, CreateVector2(0, -50), FM_Force);
		//}

		// Update bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			Step(body, deltaTime);
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Debug Stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", deltaTime), 10, 30, 20, LIME);

		DrawCircle((int) mousePosition.x, (int) mousePosition.y, 15, YELLOW);

		// Render bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			DrawCircle((int) body->position.x, (int) body->position.y, body->mass, RED);
		}

		EndDrawing();
	}
	CloseWindow();

	DestroyAllBodies();
	return 0;
}