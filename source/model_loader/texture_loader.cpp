#include "texture_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "kv.h"


bool TextureLoader::LoadFromFile(std::string filename, std::string path,unsigned int& id)
{
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, nrChannels;
	unsigned char* data = stbi_load(string(path + filename).c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else {
		cout << "Failed to load texture at: " << path << endl;
		return false;
	}

	stbi_image_free(data);
	return true;
}



