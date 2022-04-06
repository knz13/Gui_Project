#include "model_loader.h"
#include <filesystem>
#include "kv.h"

Assimp::Importer ModelLoader::m_Importer;
std::unordered_map<std::string,std::map<std::string,ModelVertices>> ModelLoader::m_ModelCache;


vector<ModelLoader::Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	
	vector<ModelLoader::Texture> Itextures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		
		mat->GetTexture(type, i, &str);
		
		
		Texture texture;
		if(!TextureLoader::LoadFromFile(string(str.C_Str()),"Resources/Models/",texture.id)){
			continue;
		}
		texture.path = "Resource/Models/" + string(str.C_Str());
		texture.type = typeName;
		//Itextures.push_back(texture); TODO
	}
	return Itextures;
}

LoadedModelResult ModelLoader::ProcessData(Drawable& model,const aiScene& scene,std::string modelFilePath,LoadingModelProperties prop)
{		
	if (scene.mNumMeshes > 0) {
		for (unsigned int i = 0; i < scene.mNumMeshes;i++) {
			prop.currentModelName = scene.mMeshes[i]->mName.C_Str();
			if(ModelLoader::AssimpGetMeshData(scene.mMeshes[i],model,prop)){
				
			}
		}
	}
	return LoadedModelResult(true);
}

LoadedModelResult ModelLoader::AssimpGetMeshData(const aiMesh* mesh,Drawable& model,LoadingModelProperties prop)
{
	aiFace* face;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> tangents;
	std::vector<unsigned int> indices;

	for (unsigned int v = 0; v < mesh->mNumVertices;v++) {
		vertices.push_back(mesh->mVertices[v].x);
		vertices.push_back(mesh->mVertices[v].y);
		vertices.push_back(mesh->mVertices[v].z);

		normals.push_back(mesh->mNormals[v].x);
		normals.push_back(mesh->mNormals[v].y);
		normals.push_back(mesh->mNormals[v].z);

		if (mesh->HasTextureCoords(0)) {
			texCoords.push_back(mesh->mTextureCoords[0][v].x);
			texCoords.push_back(mesh->mTextureCoords[0][v].y);
		} 
		else {
			texCoords.push_back(0);
			texCoords.push_back(0);
		}

		if (mesh->HasTangentsAndBitangents()) {
			tangents.push_back(mesh->mTangents[v].x);
			tangents.push_back(mesh->mTangents[v].y);
			tangents.push_back(mesh->mTangents[v].z);
		}
		else {
			tangents.push_back(0);
			tangents.push_back(0);
			tangents.push_back(0);
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
		indices.push_back(face->mIndices[0]);
		indices.push_back(face->mIndices[1]);
		indices.push_back(face->mIndices[2]);
	}
	
	model.GetVertexArray().CreateVertexBuffer(mesh->mNumVertices)
		.AddAttribute(vertices,false)
		.AddAttribute(normals,false)
		.AddAttribute(texCoords,false)
		.AddAttribute(tangents,false)
		.Generate();

	model.GetVertexArray().CreateIndexBuffer()
		.SetIndices(indices);

	prop.initializationFunc(model);


	ModelVertices vertex;
	vertex.positions = std::move(vertices);
	vertex.normals = std::move(normals);
	vertex.texCoords = std::move(texCoords);
	vertex.tangents = std::move(tangents);
	vertex.indices = std::move(indices);
	
	m_ModelCache[prop.fileName][prop.currentModelName] = std::move(vertex);

	return LoadedModelResult(true);

}



LoadedModelResult ModelLoader::LoadModel(std::string fileName,Drawable& drawable,LoadingModelProperties prop) {
    if(!std::filesystem::exists(fileName)){
		LOG("Couldn't load model at " + fileName + " because the file was not found!");
		LoadedModelResult(false);
	}

	prop.fileName = fileName;
	if(m_ModelCache.find(fileName) != m_ModelCache.end()){
		LoadedModelResult result = CopyModelFromCache(fileName,drawable,prop);
		return result;
	}


	const aiScene* modelScene = m_Importer.ReadFile(fileName, aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_ValidateDataStructure | aiProcess_Triangulate | aiProcess_EmbedTextures | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes);
	
	if (!modelScene) {
		LOG("Couldn't load model at " + fileName);
		return LoadedModelResult(false);
	}
	else {
		
		LoadedModelResult result = ProcessData(drawable,*modelScene,fileName,prop);
		if(!result){
			return LoadedModelResult(false);
		}
		return LoadedModelResult(true);
		
	}
}

LoadedModelResult ModelLoader::CopyModelFromCache(std::string cacheName,Drawable& dr,LoadingModelProperties prop) {
    
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<float> tangents;
	std::vector<unsigned int> indices;
	for (auto& [name,vertex] : m_ModelCache[cacheName]){
		

		

		positions.insert(positions.end(),vertex.positions.begin(),vertex.positions.end());
		normals.insert(normals.end(),vertex.normals.begin(),vertex.normals.end());
		texCoords.insert(texCoords.end(),vertex.texCoords.begin(),vertex.texCoords.end());
		tangents.insert(tangents.end(),vertex.tangents.begin(),vertex.tangents.end());
		indices.insert(indices.end(),vertex.indices.begin(),vertex.indices.end());

	

		

	}

	dr.GetVertexArray().CreateVertexBuffer(positions.size()/3)
		.AddAttribute(positions,false)
		.AddAttribute(normals,false)
		.AddAttribute(texCoords,false)
		.AddAttribute(tangents,false)
		.Generate();
	
	dr.GetVertexArray().CreateIndexBuffer().SetIndices(indices);

	prop.initializationFunc(dr);

	return LoadedModelResult(true);
}


