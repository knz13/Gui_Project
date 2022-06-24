#pragma once
#include "../global.h"



#define REGISTER_PANEL(x) private: static inline bool var_##x = [](){PanelManager::RegisterPanel<x>(); return true;}();




class PanelManager {
public:
	struct PanelProperties {
		std::function<void(bool&)> m_Update;
		std::function<void()> m_Init;
		bool m_IsOpen = false;
	};

	template<typename T>
	static void OpenPanel() {
		if (m_Panels.find(HelperFunctions::HashClassName<T>()) != m_Panels.end()) {
			m_Panels[HelperFunctions::HashClassName<T>()].m_IsOpen = true;
			return;
		}
		RegisterPanel<T>();
		
	};

	static void UpdatePanels() {
		for (auto& [id,panelProp] : m_Panels) {
			panelProp.m_Update(panelProp.m_IsOpen);
		}
	}
	

	template<typename T>
	static bool RegisterPanel() {
		if (m_Panels.find(HelperFunctions::HashClassName<T>()) != m_Panels.end()) {
			return true;
		}
		PanelProperties prop;
		prop.m_Update = [](bool& val) { T::Update(val); };
		T::Init();
		m_Panels[HelperFunctions::HashClassName<T>()] = prop;
		return true;
	};
private:
	static inline std::unordered_map<entt::id_type, PanelProperties> m_Panels;
};


class Panel {




};