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

    Rectangle layoutRecs[12];

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

    state.layoutRecs[0] = (Rectangle){ state.anchor01.x + 0, state.anchor01.y + 0, 304, 616 };
    state.layoutRecs[1] = (Rectangle){ state.anchor01.x + 16, state.anchor01.y + 48, 272, 192 };
    state.layoutRecs[2] = (Rectangle){ state.anchor01.x + 104, state.anchor01.y + 120, 120, 16 };
    state.layoutRecs[3] = (Rectangle){ state.anchor01.x + 104, state.anchor01.y + 80, 120, 24 };
    state.layoutRecs[4] = (Rectangle){ state.anchor01.x + 128, state.anchor01.y + 56, 120, 24 };
    state.layoutRecs[5] = (Rectangle){ state.anchor01.x + 104, state.anchor01.y + 144, 120, 16 };
    state.layoutRecs[6] = (Rectangle){ state.anchor01.x + 104, state.anchor01.y + 168, 120, 16 };
    state.layoutRecs[7] = (Rectangle){ state.anchor01.x + 104, state.anchor01.y + 192, 120, 16 };
    state.layoutRecs[8] = (Rectangle){ state.anchor01.x + 16, state.anchor01.y + 256, 272, 152 };
    state.layoutRecs[9] = (Rectangle){ state.anchor01.x + 128, state.anchor01.y + 296, 120, 16 };
    state.layoutRecs[10] = (Rectangle){ state.anchor01.x + 128, state.anchor01.y + 272, 120, 16 };
    state.layoutRecs[11] = (Rectangle){ state.anchor01.x + 104, state.anchor01.y + 216, 120, 16 };

    // Custom variables initialization

    return state;
}

void GuiLayoutName(GuiLayoutNameState *state)
{
    if (state->BodyTypeDropdownBoxEditMode) GuiLock();

    if (state->EditorWindowActive)
    {
        state->EditorWindowActive = !GuiWindowBox(state->layoutRecs[0], "Editor");
        GuiGroupBox(state->layoutRecs[1], "Body Settings");
        GuiSliderBar(state->layoutRecs[2], "Mass Min", NULL, &state->MassMinSliderValue, 0, 100);
        GuiLabel(state->layoutRecs[4], "BODY TYPE");
        GuiSliderBar(state->layoutRecs[5], "Mass Max", NULL, &state->MassMaxSliderValue, 0, 100);
        GuiSliderBar(state->layoutRecs[6], "Gravity Scale", NULL, &state->GravityScaleSliderValue, 0, 100);
        GuiSliderBar(state->layoutRecs[7], "Damping", NULL, &state->DampingSliderValue, 0, 100);
        GuiGroupBox(state->layoutRecs[8], "World Settings");
        GuiSlider(state->layoutRecs[9], "Gravitation Force", NULL, &state->GravitationForceSliderValue, 0, 100);
        GuiSlider(state->layoutRecs[10], "Gravity", NULL, &state->GravitySliderValue, 0, 100);
        GuiSliderBar(state->layoutRecs[11], "Stiffness (k)", NULL, &state->StiffnessSliderValue, 0, 100);
        if (GuiDropdownBox(state->layoutRecs[3], "DYNAMIC;KINEMATIC;STATIC", &state->BodyTypeDropdownBoxActive, state->BodyTypeDropdownBoxEditMode)) state->BodyTypeDropdownBoxEditMode = !state->BodyTypeDropdownBoxEditMode;
    }
    
    GuiUnlock();
}

#endif // GUI_LAYOUT_NAME_IMPLEMENTATION
