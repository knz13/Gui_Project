#include "model_loader.h"
#include <filesystem>

Assimp::Importer ModelLoader::m_Importer;
std::unordered_map<std::string,std::map<std::string,MeshAttribute::Vertex>> ModelLoader::m_ModelCache;


vector<ModelLoader::Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	
	vector<ModelLoader::Texture> Itextures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		
		mat->GetTexture(type, i, &str);
		
		
		ModelLoader::Texture texture;
		if(!TextureLoader::LoadFromFile(string(str.C_Str()),"Resources/Models/",texture.id)){
			continue;
		}
		texture.path = "Resource/Models/" + string(str.C_Str());
		texture.type = typeName;
		//Itextures.push_back(texture); TODO
	}
	return Itextures;
}

LoadedModelResult ModelLoader::ProcessData(const aiScene& scene,std::string modelFilePath, LoadingModelProperties prop)
{		
	prop.fileName = modelFilePath;
	if (scene.mNumMeshes > 0) {
		for (unsigned int i = 0; i < scene.mNumMeshes;i++) {
			prop.currentModelName = scene.mMeshes[i]->mName.C_Str();
			ModelLoader::AssimpGetMeshData(scene.mMeshes[i], prop);
		}
	}
	return LoadedModelResult(true);
}

LoadedModelResult ModelLoader::AssimpGetMeshData(const aiMesh* mesh, LoadingModelProperties prop)
{
	aiFace* face;
	MeshAttribute::Vertex vertex;

	for (unsigned int v = 0; v < mesh->mNumVertices;v++) {
		vertex.positions.push_back(mesh->mVertices[v].x);
		vertex.positions.push_back(mesh->mVertices[v].y);
		vertex.positions.push_back(mesh->mVertices[v].z);

		vertex.normals.push_back(mesh->mNormals[v].x);
		vertex.normals.push_back(mesh->mNormals[v].y);
		vertex.normals.push_back(mesh->mNormals[v].z);

		if (mesh->HasTextureCoords(0)) {
			vertex.texCoords.push_back(mesh->mTextureCoords[0][v].x);
			vertex.texCoords.push_back(mesh->mTextureCoords[0][v].y);
		} 
		else {
			vertex.texCoords.push_back(0);
			vertex.texCoords.push_back(0);
		}

		if (mesh->HasTangentsAndBitangents()) {
			vertex.tangents.push_back(mesh->mTangents[v].x);
			vertex.tangents.push_back(mesh->mTangents[v].y);
			vertex.tangents.push_back(mesh->mTangents[v].z);
		}
		else {
			vertex.tangents.push_back(0);
			vertex.tangents.push_back(0);
			vertex.tangents.push_back(0);
		}
	}

	if (mesh->mMaterialIndex >= 0 ) {
		/*
		aiMaterial* material = m_ModelScene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		m_Textures.insert(m_Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		m_Textures.insert(m_Textures.end(), specularMaps.begin(), specularMaps.end());
		*/
	}

	for (unsigned int i = 0; i < mesh->mNumFaces;i++) {
		face = &mesh->mFaces[i];
		vertex.indices.push_back(face->mIndices[0]);
		vertex.indices.push_back(face->mIndices[1]);
		vertex.indices.push_back(face->mIndices[2]);
	}
	
	
	

	m_ModelCache[prop.fileName][prop.currentModelName] = std::move(vertex);

	return LoadedModelResult(true);

}



LoadedModelResult ModelLoader::LoadModel(std::string fileName) {
    if(!std::filesystem::exists(fileName)){
		LOG("Couldn't load model at " + fileName + " because the file was not found!");
		return LoadedModelResult(false);
	}

	
	if(m_ModelCache.find(fileName) != m_ModelCache.end()){
		LoadedModelResult result = std::move(CopyModelFromCache(fileName));
		return result;
	}


	const aiScene* modelScene = m_Importer.ReadFile(fileName,aiProcess_FlipUVs);
	
	if (!modelScene) {
		LOG("Couldn't load model at " + fileName);
		return LoadedModelResult(false);
	}
	else {
		
		LoadedModelResult result = std::move(ProcessData(*modelScene, fileName, {}));
		if (result) {
			return CopyModelFromCache(fileName);
		}
		return result;
		
	}
}

LoadedModelResult ModelLoader::CopyModelFromCache(std::string cacheName) {
    
	MeshAttribute::Vertex vertices;

	for (auto& [name,vertex] : m_ModelCache[cacheName]){
		vertices.positions.insert(vertices.positions.end(),vertex.positions.begin(),vertex.positions.end());
		vertices.normals.insert(vertices.normals.end(),vertex.normals.begin(),vertex.normals.end());
		vertices.texCoords.insert(vertices.texCoords.end(),vertex.texCoords.begin(),vertex.texCoords.end());
		vertices.tangents.insert(vertices.tangents.end(),vertex.tangents.begin(),vertex.tangents.end());
		vertices.indices.insert(vertices.indices.end(), vertex.indices.begin(), vertex.indices.end());

	}

	LoadedModelResult result(true);
	result.vertices = vertices;

	return result;
}


