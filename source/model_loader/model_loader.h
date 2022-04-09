#pragma once
#include "texture_loader.h"
#include "../global.h"

class Mesh;
struct ModelVertices {
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> tangents;
	std::vector<unsigned int> indices;
};


struct LoadedModelResult {

	LoadedModelResult(bool state) {
		valid = state;
	}

	std::string name = "";

	operator bool() const{
		return valid;
	}

private:
	bool valid=false;

	friend class ModelLoader;
};

struct LoadingModelProperties {

	std::string modelName = "";
	std::function<void(Mesh&)> initializationFunc = [](Mesh& model){};


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

	static LoadedModelResult LoadModel(std::string fileName,Mesh& drawable,LoadingModelProperties prop = LoadingModelProperties());


private:
	
	static Assimp::Importer m_Importer;
	static std::unordered_map<std::string,std::map<std::string,ModelVertices>> m_ModelCache;


	static LoadedModelResult CopyModelFromCache(std::string cacheName,Mesh& dr,LoadingModelProperties prop);
	static vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	static LoadedModelResult ProcessData(Mesh& model,const aiScene& scene,std::string modelFilePath,LoadingModelProperties prop);
	static LoadedModelResult AssimpGetMeshData(const aiMesh* mesh,Mesh& model,LoadingModelProperties prop);




};