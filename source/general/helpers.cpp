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

void HelperFunctions::FreeImageData(unsigned char* data)
{
    stbi_image_free(data);
}

unsigned char* HelperFunctions::ReadTextureFile(std::string path, int* width, int* height, int* channels)
{   
    unsigned char* data = stbi_load(path.c_str(), width, height, channels, 0);
    if (!data) {
        throw std::runtime_error("err");
    }
    return data;
}
