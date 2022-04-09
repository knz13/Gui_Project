#pragma once
#include "../global.h"



class DrawingMode {
    KV_DRAWING_MODE

public:
    DrawingMode(){
        m_CreateFunction = [](){
            return GL_TRIANGLES;
        };
    }

protected:

    GLenum GetDrawingType(){
        return m_CreateFunction();
    };

    std::function<GLenum()> m_CreateFunction;


};


enum class LineModeType {
    Lines = 0,
    LineStrip = 1,
    LineLoop = 2
};

class LineMode : public DrawingMode {
    KV_DRAWING_MODE
public:

    LineMode(){
        SetLineType(LineModeType::Lines);
    }

    LineMode(LineModeType tp){
        SetLineType(tp);
    }

    void SetLineType(LineModeType tp){
        switch(tp){
        case LineModeType::Lines:
            m_CreateFunction = [](){ return GL_LINES; };
            break;
        case LineModeType::LineLoop:
            m_CreateFunction = [](){ return GL_LINE_LOOP;};
            break;
        case LineModeType::LineStrip:
            m_CreateFunction = [](){ return GL_LINE_STRIP;};
            break;
        }
    };

private:

    

};

enum class TriangleModeType {
    Triangle = 0,
    TriangleStrip = 1,
    TriangleFan = 2
};

class TriangleMode : public DrawingMode {
    KV_DRAWING_MODE

public:

    TriangleMode(TriangleModeType tp){
        SetTriangleType(tp);
    }

    TriangleMode(){
        SetTriangleType(TriangleModeType::Triangle);
    };

    void SetTriangleType(TriangleModeType tp){
        switch(tp){
        case TriangleModeType::Triangle:
            m_CreateFunction = [](){return GL_TRIANGLES;};
            break;
        case TriangleModeType::TriangleStrip:
            m_CreateFunction = [](){return GL_TRIANGLE_STRIP;};
            break;
        case TriangleModeType::TriangleFan:
            m_CreateFunction = [](){return GL_TRIANGLE_FAN;};
            break;
        default:
            break;
        };
    }

};

class PointsMode : public DrawingMode {
    KV_DRAWING_MODE

public:

    PointsMode(float size){
        SetPointSize(size);
    }

    PointsMode(){
        m_CreateFunction = [](){return GL_POINTS;};
    };

    void SetPointSize(float size){
        m_CreateFunction = [=](){GL_CALL(glPointSize(size)); return GL_POINTS;};
    };

};