#include "game_object.h"
#include "../kv.h"

GameObject::GameObject(entt::entity e) : Object(e)
{

}

TransformComponent& GameObject::Transform()
{
	return GetComponent<TransformComponent>();
}

void GameObject::SetHighlightState(bool state)
{
	GetComponent<GameObjectProperties>().m_HighlightState = state;
}

void GameObject::SetHighlightColor(Color color)
{
	GetComponent<GameObjectProperties>().m_HighlightColor = color;
}

Color GameObject::GetHighlightColor()
{
    return GetComponent<GameObjectProperties>().m_HighlightColor;
}



bool GameObject::IsActive()
{
	return GetComponent<GameObjectProperties>().m_Active;
}

bool GameObject::GetHighlightState()
{
	return GetComponent<GameObjectProperties>().m_HighlightState;
}


void GameObject::SetActiveState(bool state)
{
	GetComponent<GameObjectProperties>().m_Active = state;
}


void GameObject::ShowProperties()
{
    static std::vector<std::string> idsToErase;
    if (ImGui::BeginTable(GuiLayer::GetImGuiID(&GetComponent<GameObjectProperties>()).c_str(),1,ImGuiTableFlags_Borders)) {
        //ImGui::Text(("Handle = " + std::to_string((size_t)m_MasterHandle)).c_str());
        for (auto& [id, name] : Properties().GetComponentsWithIDAndName()) {
            Component* comp = GetComponentByName(name);

            if (!comp->IsVisibleInEditor()) {
                continue;
            }

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2, 0.2, 0.2, 1));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2, 0.2, 0.2, 1));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 1));
            //ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);
            //ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);



            ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPosY()));

            ImGui::TableNextColumn();
            
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x - 6, ImGui::GetCursorPos().y));

            ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);

            if ((!comp->IsRemovable())) {
                if (ImGui::BeginPopupContextWindow(GuiLayer::GetImGuiID((void*) & name).c_str(), ImGuiPopupFlags_MouseButtonRight)) {

                    if (ImGui::MenuItem("Remove Component")) {
                        idsToErase.push_back(name);
                    }

                    ImGui::EndPopup();
                }
            }

            if (ImGui::TreeNodeEx((Registry::GetComponentDisplayName(name)).c_str())) {


                ImGui::SameLine();



                if (comp->CanBeDisabled()) {
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 10, ImGui::GetCursorPos().y - 2));
                    //ImGui::Checkbox("##", pr);
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x - 10, ImGui::GetCursorPos().y + 2));
                }
                ImGui::SameLine();

                
                ImGui::Spacing();
                
                comp->ShowProperties();
                ImGui::TreePop();
                

            }

            



            ImGui::PopStyleColor(4);
            //ImGui::PopStyleVar(1);


        }
        ImGui::EndTable();
    }
    if (idsToErase.size() > 0) {
        for (auto& id : idsToErase) {
            EraseComponentByName(id);
        }
        idsToErase.clear();
    }
}




