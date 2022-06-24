#pragma once
#include "panels.h"




class LightingPanel : public Panel {
public:

	static void Update(bool& isOpen);
	static void Init();


	static void SetSkybox(ecspp::ObjectHandle handle);

private:
	static inline ecspp::ObjectHandle m_Skybox;

};