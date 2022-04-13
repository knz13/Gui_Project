#pragma once
#include "../global.h"
#include "../object/object.h"


class DrawingMode {
    KV_DRAWING_MODE

public:
    DrawingMode(Object master) : m_Master(master){
        m_CreateFunction = [](){
            return GL_TRIANGLES;
        };
    }

    ~DrawingMode(){
        m_DeleteFunc();
    };

    void ResetFuncs();
   
    std::function<GLenum()> m_CreateFunction;
    std::function<void()> m_ShowPropertiesFunc;
    std::function<void()> m_DeleteFunc;
    Object m_Master;
protected:

    GLenum GetDrawingType(){
        return m_CreateFunction();
    };


};


namespace DrawingModeType {


    void Points(DrawingMode& mode);
    void Triangles(DrawingMode& mode);
    void Lines(DrawingMode& mode);

};