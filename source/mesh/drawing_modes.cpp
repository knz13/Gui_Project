#include "drawing_modes.h"
#include "../kv.h"


void LineMode::ShowProperties() {
    
    std::vector<std::string> items = {"Line","Line Loop","Line Strip"};
    const char* current = items[this->currentIndex].c_str();

    ImGui::BulletText("Line Type");

    if(ImGui::BeginCombo(("##Line Type Combo" + GuiLayer::GetImGuiID(this)).c_str(),current)){

        int index = 0;
        for(auto& item : items){

            const bool isSelected = (currentIndex == index);
            if(ImGui::Selectable(item.c_str(),isSelected)){
                if(item == "Line"){
                    SetLineType(LineModeType::Lines);
                }
                if(item == "Line Loop"){
                    SetLineType(LineModeType::LineLoop);
                }
                if(item == "Line Strip"){
                    SetLineType(LineModeType::LineStrip);
                }
                currentIndex = index;
            }

            if(isSelected){
                ImGui::SetItemDefaultFocus();
            }

            index++;
        }
        ImGui::EndCombo();

    }
}

void TriangleMode::ShowProperties() {
    static int currentItem = 0;
    std::vector<std::string> items = {"Triangle","Triangle Strip","Triangle Fan"};
    const char* current = items[currentIndex].c_str();

    ImGui::BulletText("Triangle Type");

    if(ImGui::BeginCombo(("##Triangle Type Combo" + GuiLayer::GetImGuiID(this)).c_str(),current)){

        int index = 0;
        for(auto& item : items){

            const bool isSelected = (currentIndex == index);
            if(ImGui::Selectable(item.c_str(),isSelected)){
                if(item == "Triangle"){
                    SetTriangleType(TriangleModeType::Triangle);
                }
                if(item == "Triangle Strip"){
                    SetTriangleType(TriangleModeType::TriangleStrip);
                }
                if(item == "Triangle Fan"){
                    SetTriangleType(TriangleModeType::TriangleFan);
                }
                currentIndex = index;
            }

            if(isSelected){
                ImGui::SetItemDefaultFocus();
            }

            index++;
        }
        ImGui::EndCombo();

    }
}

void PointsMode::ShowProperties() {
    

    ImGui::BulletText("Point Size");

    ImGui::SliderFloat(GuiLayer::GetImGuiID(this).c_str(),&this->pointSize,0,10);
    
    this->SetPointSize(pointSize);

}
