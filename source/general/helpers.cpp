#include "helpers.h"
#include "../vendor/stb/stb_image.h"

bool HelperFunctions::EraseWordFromString(std::string& mainWord, std::string wordToLookFor) {
    auto iter = mainWord.find(wordToLookFor);
    
    bool foundAny = false;
    if(iter != std::string::npos){
        foundAny = true;
    }
    while (iter != std::string::npos) {
        
        mainWord.erase(iter, wordToLookFor.length());
        
        iter = mainWord.find(wordToLookFor, iter);
    }
    return foundAny;
}

std::string HelperFunctions::GenerateStringHash(void* ptr)
{
    return std::to_string(std::hash<void*>()(ptr));
}


void HelperFunctions::FreeImageData(unsigned char* data)
{
    stbi_image_free(data);
}

bool HelperFunctions::IsMetaClass(std::string className) {
    return entt::resolve(entt::hashed_string(className.c_str())).operator bool();
}

bool HelperFunctions::IsMetaFunction(std::string className, std::string funcName)
{
    if (!IsMetaClass(className)) {
        return false;
    }
    return entt::resolve(entt::hashed_string(className.c_str())).func(entt::hashed_string(funcName.c_str())).operator bool();
}

unsigned char* HelperFunctions::ReadTextureFile(std::string path, int* width, int* height, int* channels)
{   
    
    unsigned char* data = stbi_load(path.c_str(), width, height, channels, 0);
   
    if (!data) {
        DEBUG_LOG("Could not load file at " << path);
    }

    return data;
}


