#pragma once
#include "../global.h"
#include "../object/object.h"


template<typename T>
class DrawingMode;

namespace DrawingModeHelpers {


    class Null {
    public:
        Null(Object obj) {};

    };

    template<typename T>
    static std::string GetDrawingModeName() {
        std::string name = std::string(entt::type_id<T>().name());
        
        HelperFunctions::EraseWordFromString(name,"Mode");
        HelperFunctions::EraseWordFromString(name,"class ");
        HelperFunctions::EraseWordFromString(name,"struct ");
        if(auto loc = name.find_last_of(':'); loc != std::string::npos){
            name = std::string(name.begin() + loc + 1,name.end());
        }

        return name;
    }

    template<typename T>
    static entt::id_type HashDrawingModeType() {
        return entt::hashed_string(GetDrawingModeName<T>().c_str());
    }

    template<typename T>
    static void CopyDrawingModeStats(void* first,void* second){
        T& firstObj = *(T*)first;
        T& otherObj = *((T*)second);
        firstObj = otherObj;
    }
    
    template<typename T>
    static std::shared_ptr<DrawingMode<DrawingModeHelpers::Null>> GetSharedPtr(GameObject obj) {
        auto deleter = [](DrawingMode<DrawingModeHelpers::Null>* ptr){
            delete ((T*)ptr);
        };
        T* newObj = new T(obj);
        auto ptr = std::shared_ptr<DrawingMode<DrawingModeHelpers::Null>>((DrawingMode<DrawingModeHelpers::Null>*)newObj,deleter);
        return ptr;
    };

   
    static void CopyStats(std::string type,void* first,void* second){
        auto resolved = entt::resolve(entt::hashed_string(type.c_str()));

        if(resolved){
            if(auto func = resolved.func(entt::hashed_string("Copy")) ; func){
                func.invoke({},first,second);
            }
        }

    }

    static std::shared_ptr<DrawingMode<DrawingModeHelpers::Null>> GetSharedPtrFromName(std::string name,GameObject obj){
        auto resolved = entt::resolve(entt::hashed_string(name.c_str()));

        if(resolved){
            if(auto func = resolved.func(entt::hashed_string("GetSharedPtr")); func){
                if(auto result = func.invoke({},obj); result){
                    return *((std::shared_ptr<DrawingMode<DrawingModeHelpers::Null>>*)result.data());
                }
                
            }

        }
        return std::shared_ptr<DrawingMode<DrawingModeHelpers::Null>>();
    }
    
};


class DrawingModeStorage {
public:
    static const std::vector<std::string>& GetRegisteredTypes() {
        return m_RegisteredTypes;
    }

    static size_t GetTypeIndex(std::string name) {
        return std::find(m_RegisteredTypes.begin(),m_RegisteredTypes.end(),name) - m_RegisteredTypes.begin();
    }

    template<typename T>
    static void RegisterClassAsDrawingMode() {
        //registering a default to be used
        if(entt::type_id<T>().name() != entt::type_id<DrawingModeHelpers::Null>().name()){
            entt::id_type hash = DrawingModeHelpers::HashDrawingModeType<T>();
            m_RegisteredTypes.push_back(DrawingModeHelpers::GetDrawingModeName<T>());
            entt::meta<T>().type(hash).func<&DrawingModeHelpers::CopyDrawingModeStats<T>>(entt::hashed_string("Copy"));
            entt::meta<T>().type(hash).func<&DrawingModeHelpers::GetSharedPtr<T>>(entt::hashed_string("GetSharedPtr"));
            
        }
    };

private:
    static inline std::vector<std::string> m_RegisteredTypes;

};


template<typename T>
class DrawingMode {
    KV_DRAWING_MODE

public:
    DrawingMode(GameObject obj) : m_Master(obj) {
        m_CreateFunction = [](){
            return GL_TRIANGLES;
        };
    }

    virtual void ShowProperties() {};

    DrawingMode<T>& operator=(const DrawingMode<T>& other){
        m_CreateFunction = other.m_CreateFunction;


        return *this;
    };

protected:

    GLenum GetDrawingType(){
        return m_CreateFunction();
    };

    

    GameObject m_Master;
    std::function<GLenum()> m_CreateFunction;

    std::string GetName() {
        return HelperFunctions::GetClassName<T>();
    }

    
    static inline bool m_DummyVar = [](){
        DrawingModeStorage::RegisterClassAsDrawingMode<T>();
        return true;
    }();


};

class LinesMode : public DrawingMode<LinesMode> {
    KV_DRAWING_MODE
public:

    LinesMode(GameObject obj) : DrawingMode(obj){
        m_CreateFunction = [](){return GL_LINES;};
    }

   
    void ShowProperties() override;
private:
    std::string GetModeName() {
        return "Lines";
    };
    int currentIndex = 0;

};


class TrianglesMode : public DrawingMode<TrianglesMode> {
    KV_DRAWING_MODE

public:

    

    TrianglesMode(GameObject obj) : DrawingMode(obj){
        m_CreateFunction = [](){return GL_TRIANGLES;};
    };
    

    void ShowProperties() override;
private:
    int currentIndex = 0;

    

};

class PointsMode : public DrawingMode<PointsMode> {
    KV_DRAWING_MODE

public:

    PointsMode(GameObject obj);
    ~PointsMode();

    
    void ShowProperties() override;
    
private:

    
    size_t id;
    float pointSize = 1;
    Color pointColor = Color::White;
    float pointSizeRange[2];

};