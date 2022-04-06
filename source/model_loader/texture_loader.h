#pragma once
#include "../global.h"

class TextureLoader {

public:
	static bool LoadFromFile(std::string filename, std::string path, unsigned int& id);
	
};