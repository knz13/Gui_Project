#pragma once
#include <iostream>
#include <vector>
#include "../includes.h"
#include "../opengl_wrappers/texture.h"

class HelperFunctions {
public:
    static inline std::vector<std::string> SplitString(const std::string &str, const std::string &delimiter = " ", const int max_elements = 0) {
        std::vector<std::string> tokens;
        std::string::size_type start_index = 0;
        while (true) {
            std::string::size_type next_index = str.find(delimiter, start_index);
            if (next_index == std::string::npos) {
                if(str.substr(start_index) != ""){
                tokens.push_back(str.substr(start_index));
                }
                break;
            } else {
                if(str.substr(start_index, next_index - start_index) != ""){
                    tokens.push_back(str.substr(start_index, next_index - start_index));
                }
                start_index = next_index + delimiter.length();
            }
            if (max_elements > 0 && tokens.size() == max_elements - 1) {
                tokens.push_back(str.substr(start_index));
                break;
            }
        }

        return tokens;
    }


    static bool EraseWordFromString(std::string& mainWord, std::string wordToLookFor);

    template<typename T>
    static std::string GetClassName() {
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name, "class ");
        HelperFunctions::EraseWordFromString(name, "struct ");
        if (auto loc = name.find_last_of(':'); loc != std::string::npos) {
            name = std::string(name.begin() + loc + 1, name.end());
        }
        return name;
    }

    template<typename T>
    static std::string GetClassDisplayName() {
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name, "Component");
        HelperFunctions::EraseWordFromString(name, "class ");
        HelperFunctions::EraseWordFromString(name, "struct ");
        if (auto loc = name.find_last_of(':'); loc != std::string::npos) {
            name = std::string(name.begin() + loc + 1, name.end());
        }
        return name;
    }

    

    template<typename T>
    static entt::id_type HashClassName() {
        return entt::hashed_string(GetClassName<T>().c_str());
    }

    template<typename... Args>
    static entt::meta_any CallMetaFunction(std::string handle, std::string funcName,Args&&... args) {
        auto resolved = entt::resolve(entt::hashed_string(handle.c_str()));

        if (resolved) {
            if (auto func = resolved.func(entt::hashed_string(funcName.c_str())); func) {
                return func.invoke({}, args...);
            }
            return {};
        }
        return {};
    }


    template<typename TextureType>
    static Texture<TextureType> LoadTextureFromFile(std::string path) {

        if (!std::filesystem::exists(path))
        {
            return {};
        }
        int width, height, nrChannels;


        unsigned char* data = ReadTextureFile(path,&width,&height,&nrChannels);

        GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;

        Texture<TextureType> tex([=](Texture<TextureType>& texture){
            
            texture.Bind();
            GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_S, GL_REPEAT));
            GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_T, GL_REPEAT));
            GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
            GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            if (HashClassName<TextureType>() == HashClassName<Type2D>()) {
                GL_CALL(glTexImage2D(texture.GetType(), 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data));
            }

            texture.Unbind();
        });

        FreeImageData(data);

        return std::move(tex);
        
    };




private:




    static void FreeImageData(unsigned char* data);
    static unsigned char* ReadTextureFile(std::string path,int* width,int* height,int* channels);
};

namespace HelperClass {
    class Null {
    private:
        int dummy = 0;
    };

}