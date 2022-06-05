#include "properties_view.h"
#include "../kv.h"
#include "../assets/asset_object.h"

void GuiLayer::PropertiesView::Update(Window& win) {
    GuiLayer::SetupWindowStyle("Properties",[&](ImGuiWindowFlags flags){
        ImGui::Begin("Properties",0,flags );
    });
    

    

    if (GuiLayer::AnyObjectSelected()) {
        ecspp::Object selected = GuiLayer::AnyObjectSelected().GetAs<BaseObject>();
        if (selected.GetType() == "GameObject") {
            GuiLayer::AnyObjectSelected().GetAs<GameObject>().ShowProperties();
        }

        if (selected.GetTypeID() == HelperFunctions::HashClassName<GameObject>()) {
            GameObject selectedObj(selected.ID());

            ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize("Add Component").x / 2);

            GuiLayer::SetupStaticButtonStyle([]() {
                ImGui::Button("Add Component");
                });
            if (ImGui::BeginPopupContextItem("PropertiesViewContextMenuItem", ImGuiPopupFlags_MouseButtonLeft)) {

                for (auto& componentName : GameObject::GetRegisteredComponentsForType()) {
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
