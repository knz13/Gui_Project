#include "game_object.h"
#include "../kv.h"

GameObject::GameObject(entt::entity e) : BaseObject(e),RegisterStorage(e)
{

}

TransformComponent& GameObject::Transform()
{
	return GetComponent<TransformComponent>();
}

void GameObject::SetHighlightState(bool state)
{
    Storage().m_HighlightState = state;
}

void GameObject::SetHighlightColor(Color color)
{
    Storage().m_HighlightColor = color;
}

Color GameObject::GetHighlightColor()
{
    return Storage().m_HighlightColor;
}



bool GameObject::IsActive()
{
	return Storage().m_Active;
}

bool GameObject::GetHighlightState()
{
	return Storage().m_HighlightState;
}


void GameObject::SetActiveState(bool state)
{
    Storage().m_Active = state;
}




YAML::Node GameObject::Serialize()
{
    YAML::Node node;

    node["active"] = Storage().m_Active;
    node["highlight color"] = Storage().m_HighlightColor;

    return node;
}

bool GameObject::Deserialize(YAML::Node& node)
{
    if (node["active"]) {
        Storage().m_Active = node["active"].as<bool>();
    }
    if (node["hightlight color"]) {
        Storage().m_HighlightColor = node["highlight color"].as<Color>();
    }
    return true;
    
}

void GameObject::ShowProperties()
{
    static std::vector<std::string> idsToErase;
    static ImVec2 tableSize = ImVec2(0, 0);

    ImGui::SetCursorPosX(0);
    if (ImGui::BeginTable(GuiLayer::GetImGuiID(&Storage()).c_str(),1,ImGuiTableFlags_Borders | ImGuiTableFlags_ContextMenuInBody,ImVec2(ImGui::GetWindowSize().x,0),10)) {
        //ImGui::Text(("Handle = " + std::to_string((size_t)m_MasterHandle)).c_str());

        

        for (auto& name : GetComponentsNames()) {
            
            auto* comp = GetComponentByName(name).GetAs<GameComponent>();

            if (!comp->IsVisibleInEditor()) {
                continue;
            }

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2, 0.2, 0.2, 1));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2, 0.2, 0.2, 1));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 1));
            //ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);
            //ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);

            

            
            ImGui::TableNextColumn();

            
            
     
            
            ImGui::SetCursorPosX(2);
            ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
            bool selected;
            
            GuiLayer::SetupStaticTreeNodeStyle([&]() {
                selected = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Selected |ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_AllowItemOverlap);
            });
            
                
            if (selected) {


                ImGui::SameLine();



                if (comp->CanBeDisabled()) {
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 10, ImGui::GetCursorPos().y - 2));
                    ImGui::CheckboxEx("##", &comp->m_BaseComponentActiveState);
                    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x - 10, ImGui::GetCursorPos().y + 2));
                }
                ImGui::SameLine();

                
                ImGui::Spacing();
                
                comp->ShowProperties();
                ImGui::TreePop();
                

                ImGui::InvisibleButton("##", ImVec2(1, 5));
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




