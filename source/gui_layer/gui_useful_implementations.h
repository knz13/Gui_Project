#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_internal.h"
#include <iostream>
#include <string>

static ImVector<ImRect> s_GroupPanelLabelStack;
namespace ImGui {
    void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0,0));

    void EndGroupPanel();


    void BeginAlignRight();

    void EndAlignRight();

    void AlignNextRight();

    void AlignedText(std::string text);

    void AlignNextCheckboxRight();

    void DragTextFloat(std::string text,float* val,float speed = 1,float min=0,float max=0);

    void Drag3AxisFloat(std::string text,float* values,float speed = 1,float min=0,float max = 0);

    bool CheckboxEx(const char* label, bool* v);
}




