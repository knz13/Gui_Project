#pragma once
#include "../global.h"
#include "../object/object.h"


class DrawingMode {
    KV_DRAWING_MODE

public:
    DrawingMode() {
        m_CreateFunction = [](){
            return GL_TRIANGLES;
        };
    }

    void SetMaster(Object master) {
        m_Master = master.ID();
    };

    ~DrawingMode(){
    };

    void ResetFuncs();
   
    std::function<GLenum()> m_CreateFunction;
    std::function<void()> m_ShowPropertiesFunc;
    std::function<void()> m_DeleteFunc;
    Object GetMasterObject() {
        return Object(m_Master);
    }
protected:
    entt::entity m_Master = entt::null;


    GLenum GetDrawingType(){
        return m_CreateFunction();
    };


};


namespace DrawingModeType {


    void Points(DrawingMode& mode);
    void Triangles(DrawingMode& mode);
    void Lines(DrawingMode& mode);

};