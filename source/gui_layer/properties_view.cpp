#include "properties_view.h"
#include "../kv.h"

void GuiLayer::PropertiesView::Update(Window& win) {
    GuiLayer::SetupWindowStyle([&](ImGuiWindowFlags flags){
        ImGui::Begin("Properties",0,flags );
    });

    if(GameView::AnyObjectSelected()){
        Object(GameView::AnyObjectSelected().objectID).Properties().CallShowPropertiesFunctions();
    }

    ImGui::End();
}
