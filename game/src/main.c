#include <stdlib.h>
#include <assert.h>

#include "raylib.h"
#include "raymath.h"

#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "force.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define BODIES_PER_CLICK 50

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
		if(IsMouseButtonPressed(0)) {
			switch(GetRandomValue(0, 2)) {
				case 0:
					// Explosions in a specific direction (up)
					{
						float angle = -90.0f; // GetRandomFloatValue(0, 360);
						for(int i = 0; i < BODIES_PER_CLICK; i++) {
							ncBody* body = CreateBody();
							body->position = mousePosition;
							body->prevPosition = mousePosition;
							body->prevPrevPosition = mousePosition;
							body->mass = GetRandomFloatValue(1.0f, 5.0f);
							body->inverseMass = 1 / body->mass;
							body->bodyType = BT_Dynamic;
							body->damping = 0.5f;
							body->gravityScale = 6.0f;
							body->color = ColorFromHSV(GetRandomFloatValue(0, 360), GetRandomFloat01(), GetRandomFloat01());
							Vector2 force = Vector2Scale(GetVector2FromAngle((angle + GetRandomFloatValue(-30.0f, 30.0f)) * DEG2RAD), GetRandomFloatValue(1000, 2000));
							ApplyForce(body, force, FM_Impulse);
						}
					}
					break;
				case 1:
					// Explosions in a circle
					for(int i = 0; i < BODIES_PER_CLICK; i++) {
						ncBody* body = CreateBody();
						body->position = mousePosition;
						body->prevPosition = mousePosition;
						body->prevPrevPosition = mousePosition;
						body->mass = GetRandomFloatValue(1.0f, 5.0f);
						body->inverseMass = 1 / body->mass;
						body->bodyType = BT_Dynamic;
						body->damping = 0.5f;
						body->gravityScale = 6.0f;
						body->color = ColorFromHSV(GetRandomFloatValue(0, 360), GetRandomFloat01(), GetRandomFloat01());
						Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(1000, 2000));
						ApplyForce(body, force, FM_Impulse);
					}
					break;
				default:
					// Explosions in a hexagon-like shape
					{
						float angle = 0.0f;
						for(int i = 0; i < BODIES_PER_CLICK; i++) {
							switch(i % 6) {
								case 0:
									angle = -45.0f;
									break;
								case 1:
									angle = 0.0f;
									break;
								case 2:
									angle = 45.0f;
									break;
								case 3:
									angle = 135.0f;
									break;
								case 4:
									angle = 180.0f;
									break;
								default:
									angle = 235.0f;
									break;
							}

							ncBody* body = CreateBody();
							body->position = mousePosition;
							body->prevPosition = mousePosition;
							body->prevPrevPosition = mousePosition;
							body->mass = GetRandomFloatValue(1.0f, 5.0f);
							body->inverseMass = 1 / body->mass;
							body->bodyType = BT_Dynamic;
							body->damping = 0.5f;
							body->gravityScale = 6.0f;
							body->color = ColorFromHSV(GetRandomFloatValue(0, 360), GetRandomFloat01(), GetRandomFloat01());
							Vector2 force = Vector2Scale(GetVector2FromAngle((angle + GetRandomFloatValue(-5.0f, 5.0f)) * DEG2RAD), GetRandomFloatValue(1000, 2000));
							ApplyForce(body, force, FM_Impulse);
						}
					}
					break;
			}
		}

		if(IsKeyPressed(KEY_R)) {
			DestroyAllBodies();
		}

		// Apply force
		ApplyGravitation(ncBodies, 30.0f);

		// Update bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			Step(body, deltaTime);
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		DrawCircle((int) mousePosition.x, (int) mousePosition.y, 15, YELLOW);

		// Render bodies
		for(ncBody* body = ncBodies; body != NULL; body = body->next) {
			DrawCircle((int) body->position.x, (int) body->position.y, body->mass, body->color);
			DrawLine((int) body->prevPrevPosition.x, (int) body->prevPrevPosition.y, (int) body->prevPosition.x, (int) body->prevPosition.y, body->color);
			DrawLine((int) body->prevPosition.x, (int) body->prevPosition.y, (int) body->position.x, (int) body->position.y, body->color);
		}

		// Debug Stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", deltaTime), 10, 30, 20, LIME);

		EndDrawing();
	}
	CloseWindow();

	DestroyAllBodies();
	return 0;
}