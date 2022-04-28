#include "properties_view.h"
#include "../kv.h"
#include "../assets/asset_object.h"

void GuiLayer::PropertiesView::Update(Window& win) {
    GuiLayer::SetupWindowStyle("Properties",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Properties",0,flags );
    });
    

    if (ImGui::Button("Serialize")) {
        if (!std::filesystem::exists("Scenes")) {
            std::filesystem::create_directories("Scenes");
            
        }
        ObjectPropertyRegister::SerializeScene("Scenes/hello.yml");
    }

    if (GuiLayer::AnyObjectSelected()) {
        Object selected = GuiLayer::AnyObjectSelected().GetAsObject();
        selected.ShowObjectProperties();

        if (selected.GetTypeOfObject() == HelperFunctions::HashClassName<GameObject>()) {
            GameObject selectedObj(selected.ID());

            ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize("Add Component").x / 2);

            GuiLayer::SetupStaticButtonStyle([]() {
                ImGui::Button("Add Component");
                });
            if (ImGui::BeginPopupContextItem("PropertiesViewContextMenuItem", ImGuiPopupFlags_MouseButtonLeft)) {

                for (auto& componentName : GameObject::GetRegisteredComponents()) {
                    if (!selected.HasComponent(componentName)) {
                        if (ImGui::MenuItem(componentName.c_str())) {
                            selectedObj.AddComponentByName(componentName);
                        }
                    }
                }



                ImGui::EndPopup();
            }
        }

    }

    ImGui::End();
}
