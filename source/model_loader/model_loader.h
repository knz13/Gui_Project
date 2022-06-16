#pragma once
#include "texture_loader.h"
#include "../global.h"
#include "../mesh/mesh.h"

class Mesh;


struct LoadedModelResult {

	LoadedModelResult(bool state) {
		valid = state;
	}

	std::string name = "";

	operator bool() const{
		return valid;
	}

	MeshAttribute::Vertex vertices;
private:
	bool valid=false;

	friend class ModelLoader;
};

struct LoadingModelProperties {

	std::string modelName = "";


private:
	std::string currentModelName = "";
	std::string fileName = "";

	friend class ModelLoader;

};


class ModelLoader {
	struct Texture {
		unsigned int id;
		string type;
		string path;
	};
	
public:

	static LoadedModelResult LoadModel(std::string fileName);


private:
	
	static Assimp::Importer m_Importer;
	static std::unordered_map<std::string,std::map<std::string,MeshAttribute::Vertex>> m_ModelCache;


	static LoadedModelResult CopyModelFromCache(std::string cacheName);
	static vector<ModelLoader::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	static LoadedModelResult ProcessData(const aiScene& scene,std::string modelFilePath,LoadingModelProperties prop);
	static LoadedModelResult AssimpGetMeshData(const aiMesh* mesh,LoadingModelProperties prop);




};