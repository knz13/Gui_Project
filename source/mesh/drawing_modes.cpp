#include "drawing_modes.h"
#include "../kv.h"







void TrianglesMode::ShowProperties() {
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
                    m_CreateFunction = [](){return GL_TRIANGLES;};
                }
                if(item == "Triangle Strip"){
                    m_CreateFunction = [](){return GL_TRIANGLE_STRIP;};
                }
                if(item == "Triangle Fan"){
                    m_CreateFunction = [](){return GL_TRIANGLE_FAN;};
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
}


void LinesMode::ShowProperties() {
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
                    m_CreateFunction = [](){ return GL_LINES;};
                }
                if(item == "Line Loop"){
                    m_CreateFunction = [](){ return GL_LINE_LOOP;};
                }
                if(item == "Line Strip"){
                    m_CreateFunction = [](){ return GL_LINE_STRIP;};
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

    ImGui::SliderFloat(GuiLayer::GetImGuiID(&pointSize).c_str(),&pointSize,pointSizeRange[0],pointSizeRange[1]);

    ImGui::BulletText("Point Color");

    ImGui::ColorEdit3(GuiLayer::GetImGuiID(&pointColor).c_str(),&pointColor.Normalized().x,ImGuiColorEditFlags_InputRGB);
}

PointsMode::PointsMode(Object obj) : DrawingMode(obj) {
    m_CreateFunction = [](){return GL_POINTS;};

    id = obj.GetComponent<Mesh>().PreDrawn().Connect([&,obj](Mesh& mesh,Shader&,const glm::mat4& mvp) mutable -> void {
        Shader& shader = Window::GetCurrentWindow().Create().CachedShader("default_shaders/points_shader",nullptr);
    
        shader.Bind();
        shader.SetUniformMat4f("MVP",mvp);
        if(Window::GetCurrentWindow().GetCurrentCamera()){
            float distance = glm::distance(Window::GetCurrentWindow().GetCurrentCamera().GetAsObject().Transform().GetPosition(),obj.Transform().GetPosition());
            shader.SetUniform1f("cameraDistance",distance);
        }
        shader.SetUniform1f("pointSize",pointSize);
        shader.SetUniform3f("pointColor",pointColor.Normalized().x,pointColor.Normalized().y,pointColor.Normalized().z);

    });

    GL_CALL(glGetFloatv(GL_POINT_SIZE_RANGE,pointSizeRange));
}

PointsMode::~PointsMode() {
    m_Master.GetComponent<Mesh>().PreDrawn().Disconnect(id);
}
