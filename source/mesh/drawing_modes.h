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

    void ShowDerivedProperties() {
        ShowProperties();
    }

    std::string GetName() {
        return GetDrawingModeName();
    }

    virtual std::string GetDrawingModeName() { return "None";};
    virtual void ShowProperties() {};
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

    std::string GetDrawingModeName() override{
        return "Lines";
    }
    void ShowProperties() override;
private:
    int currentIndex = 0;

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
    std::string GetDrawingModeName() override{
        return "Triangles";
    }

    void ShowProperties() override;
private:
    int currentIndex = 0;


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

    void ShowProperties() override;
    std::string GetDrawingModeName() override{
        return "Points";
    }
private:

    float pointSize = 1;

};