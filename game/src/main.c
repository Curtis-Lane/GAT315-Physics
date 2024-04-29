#include <stdlib.h>
#include <assert.h>

#include "raylib.h"
#include "raymath.h"

#include "body.h"
#include "mathf.h"
#include "world.h"

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
			Body* body = CreateBody();
			body->position = mousePosition;
			body->velocity = CreateVector2(GetRandomFloatValue(-5.0f, 5.0f), GetRandomFloatValue(-5.0f, 5.0f));
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Debug Stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", deltaTime), 10, 30, 20, LIME);

		DrawCircle((int) mousePosition.x, (int) mousePosition.y, 15, YELLOW);

		{
			Body* body = bodies;
			for(int i = 0; i < bodyCount; i++) {
				body->position = Vector2Add(body->position, body->velocity);
				DrawCircle((int) body->position.x, (int) body->position.y, 15, RED);

				body = body->next;
			}
		}

		EndDrawing();
	}
	CloseWindow();

	DestroyAllBodies();
	return 0;
}