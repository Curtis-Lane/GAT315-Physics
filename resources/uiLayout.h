/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   MODULE USAGE:
*       #define GUI_LAYOUT_NAME_IMPLEMENTATION
*       #include "gui_layout_name.h"
*
*       INIT: GuiLayoutNameState state = InitGuiLayoutName();
*       DRAW: GuiLayoutName(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LAYOUT_NAME_H
#define GUI_LAYOUT_NAME_H

typedef struct {
    Vector2 anchor01;
    
    bool EditorWindowActive;
    float MassMinSliderValue;
    bool BodyTypeDropdownBoxEditMode;
    int BodyTypeDropdownBoxActive;
    float MassMaxSliderValue;
    float GravityScaleSliderValue;
    float DampingSliderValue;
    float GravitationForceSliderValue;
    float GravitySliderValue;
    float StiffnessSliderValue;
    float RestitutionSliderValue;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiLayoutNameState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiLayoutNameState InitGuiLayoutName(void);
void GuiLayoutName(GuiLayoutNameState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_LAYOUT_NAME_H

/***********************************************************************************
*
*   GUI_LAYOUT_NAME IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_LAYOUT_NAME_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiLayoutNameState InitGuiLayoutName(void)
{
    GuiLayoutNameState state = { 0 };

    state.anchor01 = (Vector2){ 776, 56 };
    
    state.EditorWindowActive = true;
    state.MassMinSliderValue = 0.0f;
    state.BodyTypeDropdownBoxEditMode = false;
    state.BodyTypeDropdownBoxActive = 0;
    state.MassMaxSliderValue = 0.0f;
    state.GravityScaleSliderValue = 0.0f;
    state.DampingSliderValue = 0.0f;
    state.GravitationForceSliderValue = 0.0f;
    state.GravitySliderValue = 0.0f;
    state.StiffnessSliderValue = 0.0f;
    state.RestitutionSliderValue = 0.0f;

    // Custom variables initialization

    return state;
}

void GuiLayoutName(GuiLayoutNameState *state)
{
    if (state->BodyTypeDropdownBoxEditMode) GuiLock();

    if (state->EditorWindowActive)
    {
        state->EditorWindowActive = !GuiWindowBox((Rectangle){ state->anchor01.x + 0, state->anchor01.y + 0, 304, 616 }, "Editor");
        GuiGroupBox((Rectangle){ state->anchor01.x + 16, state->anchor01.y + 48, 272, 216 }, "Body Settings");
        GuiSliderBar((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 120, 120, 16 }, "Mass Min", NULL, &state->MassMinSliderValue, 0, 100);
        GuiLabel((Rectangle){ state->anchor01.x + 128, state->anchor01.y + 56, 120, 24 }, "BODY TYPE");
        GuiSliderBar((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 144, 120, 16 }, "Mass Max", NULL, &state->MassMaxSliderValue, 0, 100);
        GuiSliderBar((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 168, 120, 16 }, "Gravity Scale", NULL, &state->GravityScaleSliderValue, 0, 100);
        GuiSliderBar((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 192, 120, 16 }, "Damping", NULL, &state->DampingSliderValue, 0, 100);
        GuiGroupBox((Rectangle){ state->anchor01.x + 16, state->anchor01.y + 280, 272, 152 }, "World Settings");
        GuiSlider((Rectangle){ state->anchor01.x + 128, state->anchor01.y + 312, 120, 16 }, "Gravitation Force", NULL, &state->GravitationForceSliderValue, 0, 100);
        GuiSlider((Rectangle){ state->anchor01.x + 128, state->anchor01.y + 288, 120, 16 }, "Gravity", NULL, &state->GravitySliderValue, 0, 100);
        GuiSliderBar((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 216, 120, 16 }, "Stiffness (k)", NULL, &state->StiffnessSliderValue, 0, 100);
        GuiSliderBar((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 240, 120, 16 }, "Restitution", NULL, &state->RestitutionSliderValue, 0, 100);
        if (GuiDropdownBox((Rectangle){ state->anchor01.x + 104, state->anchor01.y + 80, 120, 24 }, "DYNAMIC;KINEMATIC;STATIC", &state->BodyTypeDropdownBoxActive, state->BodyTypeDropdownBoxEditMode)) state->BodyTypeDropdownBoxEditMode = !state->BodyTypeDropdownBoxEditMode;
    }
    
    GuiUnlock();
}

#endif // GUI_LAYOUT_NAME_IMPLEMENTATION
