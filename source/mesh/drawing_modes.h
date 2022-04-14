#pragma once
#include "../global.h"
#include "../object/object.h"


class DrawingMode {
    KV_DRAWING_MODE

public:
    DrawingMode(Object obj) : m_Master(obj) {
        m_CreateFunction = [](){
            return GL_TRIANGLES;
        };
    }

    virtual void ShowProperties() {};
protected:

    GLenum GetDrawingType(){
        return m_CreateFunction();
    };

    Object m_Master;
    std::function<GLenum()> m_CreateFunction;

    template<typename T,typename ...Args>
    static std::shared_ptr<DrawingMode> GetSharedPtr(Args&&... args) {
        auto deleter = [](DrawingMode* ptr){
            delete ((T*)ptr);
        };
        return std::shared_ptr<DrawingMode>(new T(args...),deleter);
    };

};

class LineMode : public DrawingMode {
    KV_DRAWING_MODE
public:

    LineMode(Object obj) : DrawingMode(obj){
        m_CreateFunction = [](){return GL_LINES;};
    }

   
    void ShowProperties() override;
private:
    int currentIndex = 0;

};


class TriangleMode : public DrawingMode {
    KV_DRAWING_MODE

public:

    

    TriangleMode(Object obj) : DrawingMode(obj){
        m_CreateFunction = [](){return GL_TRIANGLES;};
    };
    

    void ShowProperties() override;
private:
    int currentIndex = 0;


};

class PointsMode : public DrawingMode {
    KV_DRAWING_MODE

public:

    PointsMode(Object obj);
    ~PointsMode();

    
    void ShowProperties() override;
    
private:
    size_t id;
    float pointSize = 1;
    Color pointColor = Color::White;
    float pointSizeRange[2];

};