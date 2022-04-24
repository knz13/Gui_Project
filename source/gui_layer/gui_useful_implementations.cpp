#include "gui_useful_implementations.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "../vendor/imgui/imstb_textedit.h"
#include "../vendor/imgui/imstb_rectpack.h"
#include "../kv.h"
#include "GLFW/glfw3.h"
#include <iomanip>

void BeginGroupPanel(const char* name, const ImVec2& size) {
    ImGui::BeginGroup();

    auto cursorPos = ImGui::GetCursorScreenPos();
    auto itemSpacing = ImGui::GetStyle().ItemSpacing;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    auto frameHeight = ImGui::GetFrameHeight();
    ImGui::BeginGroup();

    ImVec2 effectiveSize = size;
    if (size.x < 0.0f)
        effectiveSize.x = ImGui::GetContentRegionAvail().x;
    else
        effectiveSize.x = size.x;
    ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::BeginGroup();
    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::TextUnformatted(name);
    auto labelMin = ImGui::GetItemRectMin();
    auto labelMax = ImGui::GetItemRectMax();
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
    ImGui::BeginGroup();

    //ImGui::GetWindowDrawList()->AddRect(labelMin, labelMax, IM_COL32(255, 0, 255, 255));

    ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
    ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->WorkRect.Max.x          -= frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->InnerRect.Max.x         -= frameHeight * 0.5f;
#else
    ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x -= frameHeight * 0.5f;
#endif
    ImGui::GetCurrentWindow()->Size.x                   -= frameHeight;

    auto itemWidth = ImGui::CalcItemWidth();
    ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

    s_GroupPanelLabelStack.push_back(ImRect(labelMin, labelMax));    
}

void ImGui::EndGroupPanel() {
    ImGui::PopItemWidth();

    auto itemSpacing = ImGui::GetStyle().ItemSpacing;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

    auto frameHeight = ImGui::GetFrameHeight();

    ImGui::EndGroup();

    //ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

    ImGui::EndGroup();

    ImGui::SameLine(0.0f, 0.0f);
    ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
    ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

    ImGui::EndGroup();

    auto itemMin = ImGui::GetItemRectMin();
    auto itemMax = ImGui::GetItemRectMax();
    //ImGui::GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

    auto labelRect = s_GroupPanelLabelStack.back();
    s_GroupPanelLabelStack.pop_back();

    ImVec2 halfFrame = ImVec2(frameHeight * 0.25f* 0.5f, frameHeight* 0.5f);
    ImRect frameRect = ImRect(itemMin.x + halfFrame.x,itemMin.y + halfFrame.y, itemMax.x - halfFrame.x,itemMax.y - 0.0f);
    labelRect.Min.x -= itemSpacing.x;
    labelRect.Max.x += itemSpacing.x;
    for (int i = 0; i < 4; ++i)
    {
        switch (i)
        {
            // left half-plane
            case 0: ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(labelRect.Min.x, FLT_MAX), true); break;
            // right half-plane
            case 1: ImGui::PushClipRect(ImVec2(labelRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true); break;
            // top
            case 2: ImGui::PushClipRect(ImVec2(labelRect.Min.x, -FLT_MAX), ImVec2(labelRect.Max.x, labelRect.Min.y), true); break;
            // bottom
            case 3: ImGui::PushClipRect(ImVec2(labelRect.Min.x, labelRect.Max.y), ImVec2(labelRect.Max.x, FLT_MAX), true); break;
        }

        ImGui::GetWindowDrawList()->AddRect(
            frameRect.Min, frameRect.Max,
            ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
            halfFrame.x);

        ImGui::PopClipRect();
    }

    ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
    ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->WorkRect.Max.x          += frameHeight * 0.5f;
    ImGui::GetCurrentWindow()->InnerRect.Max.x         += frameHeight * 0.5f;
#else
    ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += frameHeight * 0.5f;
#endif
    ImGui::GetCurrentWindow()->Size.x                   += frameHeight;

    ImGui::Dummy(ImVec2(0.0f, 0.0f));

    ImGui::EndGroup();
}

void ImGui::BeginAlignRight() {
    ImGui::BeginGroup();
}



void ImGui::EndAlignRight() {

    

    ImGui::EndGroup();


    
}

void ImGui::AlignNextRight() {
    ImGui::SetCursorPosX(ImGui::CalcTextSize("SuperBigText").x + ImGui::GetWindowSize().x/8);
    ImGui::SetNextItemWidth(ImGui::GetWindowSize().x -ImGui::GetCursorPosX() - 8);
}

void ImGui::TextCenterAligned(std::string text)
{
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(text.c_str()).x / 2);

    ImGui::Text(text.c_str());
}

bool ImGui::TextEditCenterAligned(std::string* textPtr)
{
    ImGui::InputText(GuiLayer::GetImGuiID(textPtr).c_str(), textPtr);
}

void ImGui::AlignedText(std::string text) {
    ImGui::SetCursorPosX(2);
    ImGui::BulletText(text.c_str());
}

void ImGui::AlignNextCheckboxRight() {
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 8 - 19);
}

void ImGui::DragTextFloat(std::string text,float* val,float speed,float min,float max) {

    
    static std::unordered_map<float*,bool> selecteds;
    static float lastPosX = 0;

    if(selecteds.find(val) == selecteds.end()){
        selecteds.emplace();
    }

    bool& selected = selecteds[val];

    ImGui::Text((text).c_str());
    bool wasSelected = selected;
    if(ImGui::IsItemClicked()){
        selected = true;
    }
    if(ImGui::IsItemHovered()){
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    }

    if(selected){
        if(selected != wasSelected){
            double x,y;
            glfwGetCursorPos(Window::GetCurrentWindow().GetContextPointer(),&x,&y);
            lastPosX = x;
        }
        if(IsMouseDown(ImGuiMouseButton_Left)){
            glfwSetInputMode(Window::GetCurrentWindow().GetContextPointer(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
            double x,y;
            glfwGetCursorPos(Window::GetCurrentWindow().GetContextPointer(),&x,&y);
            if(min < max){
                if(*val > min || *val < max){
                    *val += (x - lastPosX) * Window::GetCurrentWindow().GetDeltaTime()*speed * 100;
                }
            }
            else{
                *val += (x - lastPosX) * Window::GetCurrentWindow().GetDeltaTime() * speed * 100;
            }
            lastPosX = x;
            
        }
        if(IsMouseReleased(ImGuiMouseButton_Left)){
            glfwSetInputMode(Window::GetCurrentWindow().GetContextPointer(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
            selected = false;
        }
        
    }
    ImGui::SameLine();
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << *val;
    ImGui::SetNextItemWidth(ImGui::CalcTextSize(stream.str().c_str()).x);
    //ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
    ImGui::InputFloat(GuiLayer::GetImGuiID(val).c_str(),val,0.0f,0,"%.1f");
    

}


void ImGui::Drag3AxisFloat(std::string text, float* values, float speed, float min, float max) {
    
}

bool ImGui::CheckboxEx(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 1.5f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }
    

    const float boxSize = IM_FLOOR(square_sz * 0.65f);
    const float boxOffsetHorz = IM_FLOOR(style.ItemSpacing.x * 1.2f);
    const float boxOffsetVert = IM_FLOOR(std::floor(0.5f * (square_sz - boxSize)));

    ImRect check_bb(pos + ImVec2(boxOffsetHorz, boxOffsetVert), pos + ImVec2(boxOffsetHorz + boxSize, boxOffsetVert + boxSize));
    ImU32 check_col = GetColorU32((g.CurrentItemFlags & ImGuiItemFlags_Disabled) ? ImGuiCol_FrameBg : ImGuiCol_Text);
    RenderNavHighlight(total_bb, id);

    ImVec4 border_col = style.Colors[hovered ? ImGuiCol_Text : ImGuiCol_CheckMark];
    border_col.w *= 0.5f;

    ImGui::PushStyleColor(ImGuiCol_Border, border_col);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    RenderFrame(check_bb.Min, check_bb.Max, ImColor(0, 0, 0, 0), true, 0.0f);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    if (g.CurrentItemFlags & ImGuiItemFlags_MixedValue)
    {
        // Undocumented tristate/mixed/indeterminate checkbox (#2644)
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 7.0f)), ImMax(1.0f, IM_FLOOR(square_sz / 7.0f)));
        window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, 0.0f);
    }
    else if (*v)
    {
        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
        ImVec2 checkMarkPts[3] = {
            check_bb.Min + ImVec2(pad, pad + boxSize * 0.3f),
            check_bb.Min + ImVec2(boxSize * 0.45f, boxSize - pad),
            check_bb.Min + ImVec2(boxSize - pad, pad),
        };
        window->DrawList->AddPolyline(checkMarkPts, 3, ImColor(check_col), false, 2.5f);
    }

    check_bb = ImRect(pos, pos + ImVec2(square_sz, square_sz));

    if (g.LogEnabled)
        LogRenderedText(&total_bb.Min, *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f)
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.CurrentItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}
