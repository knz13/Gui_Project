#include "drawing_modes.h"
#include "../kv.h"








void DrawingMode::ResetFuncs() {
    m_CreateFunction = [](){
        return GL_TRIANGLES;  
    };
    m_ShowPropertiesFunc = [](){

    };
    m_DeleteFunc = [](){

    };
}

void DrawingModeType::Triangles(DrawingMode& mode) {
    mode.ResetFuncs();
    mode.m_CreateFunction = [](){
        return GL_TRIANGLES;
    };

    mode.m_ShowPropertiesFunc = [&](){
        static int currentItem = 0;
        std::vector<std::string> items = {"Triangle","Triangle Strip","Triangle Fan"};
        const char* current = items[currentItem].c_str();

        ImGui::BulletText("Triangle Type");

        if(ImGui::BeginCombo(("##Triangle Type Combo" + GuiLayer::GetImGuiID(&currentItem)).c_str(),current)){

            int index = 0;
            for(auto& item : items){

                const bool isSelected = (currentItem == index);
                if(ImGui::Selectable(item.c_str(),isSelected)){
                    if(item == "Triangle"){
                        mode.m_CreateFunction = [](){return GL_TRIANGLES;};
                    }
                    if(item == "Triangle Strip"){
                        mode.m_CreateFunction = [](){return GL_TRIANGLE_STRIP;};
                    }
                    if(item == "Triangle Fan"){
                        mode.m_CreateFunction = [](){return GL_TRIANGLE_FAN;};
                    }
                    currentItem = index;
                }

                if(isSelected){
                    ImGui::SetItemDefaultFocus();
                }

                index++;
            }
            ImGui::EndCombo();

        }
    };
}

void DrawingModeType::Lines(DrawingMode& mode) {
    mode.ResetFuncs();
    mode.m_CreateFunction = [](){ return GL_LINES;};
    mode.m_ShowPropertiesFunc = [&](){
        static int currentIndex = 0;
        std::vector<std::string> items = {"Line","Line Loop","Line Strip"};
        const char* current = items[currentIndex].c_str();

        ImGui::BulletText("Line Type");

        if(ImGui::BeginCombo(("##Line Type Combo" + GuiLayer::GetImGuiID(&currentIndex)).c_str(),current)){

            int index = 0;
            for(auto& item : items){

                const bool isSelected = (currentIndex == index);
                if(ImGui::Selectable(item.c_str(),isSelected)){
                    if(item == "Line"){
                        mode.m_CreateFunction = [](){ return GL_LINES;};
                    }
                    if(item == "Line Loop"){
                        mode.m_CreateFunction = [](){ return GL_LINE_LOOP;};
                    }
                    if(item == "Line Strip"){
                        mode.m_CreateFunction = [](){ return GL_LINE_STRIP;};
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
    };
}

void DrawingModeType::Points(DrawingMode& mode) {
    mode.ResetFuncs();
    mode.m_CreateFunction = [](){return GL_POINTS;};
    
    mode.m_ShowPropertiesFunc = [&](){
        static Color pointColor = Color::White;
        static float pointSize = 1;
        static float pointSizeRange[2]; 
        static bool firstLoop = true;
        if(firstLoop){
            uint32_t id = mode.GetMasterObject().GetComponent<Mesh>().PreDrawn().Connect([&](Mesh& mesh,Shader&){
            mesh.SetShader("default_shaders/points_shader");
            if(Window::GetCurrentWindow().GetCurrentCamera()){
                float distance = glm::distance(Window::GetCurrentWindow().GetCurrentCamera().GetAsObject().Transform().GetPosition(),mode.GetMasterObject().Transform().GetPosition());
                mesh.GetShader().SetUniform1f("cameraDistance",distance);
            }
            mesh.GetShader().SetUniform1f("pointSize",pointSize);
            mesh.GetShader().SetUniform3f("pointColor",pointColor.Normalized().x,pointColor.Normalized().y,pointColor.Normalized().z);

            });

            GL_CALL(glGetFloatv(GL_POINT_SIZE_RANGE,pointSizeRange));
            mode.m_DeleteFunc = [&](){
                mode.GetMasterObject().GetComponent<Mesh>().PreDrawn().Disconnect(id);
            };
            firstLoop = false;
        }

        ImGui::BulletText("Point Size");

        ImGui::SliderFloat(GuiLayer::GetImGuiID(&pointSize).c_str(),&pointSize,pointSizeRange[0],pointSizeRange[1]);

        ImGui::BulletText("Point Color");
        ImGui::SameLine();

        ImGui::ColorEdit3(GuiLayer::GetImGuiID(&pointColor).c_str(),&pointColor.Normalized().x,ImGuiColorEditFlags_InputRGB);

    };
}
