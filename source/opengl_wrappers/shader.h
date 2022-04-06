#pragma once
#include "../global.h"

class Shader;
struct ShaderCreationProperties {
    ShaderCreationProperties(Shader& master);

    ShaderCreationProperties& AddShader(ShaderType type,std::string source);
    bool Generate();

private:
    Shader* m_Master;
    std::unordered_map<ShaderType,std::vector<std::string>> m_Shaders;
};

struct ShaderUniformContainer {
    int location = -1;
    std::string type = "";
    int size = -1;
};


class Shader {
    KV_CLASS
public:

    Shader();

    ShaderCreationProperties CreateNew();

    bool SetUniform1f(const string& name, float value);
	bool SetUniform1i(const string& name, int value);
    bool SetUniform2f(const string& name, float v0,float v1);
	bool SetUniform3f(const string& name, float v0, float v1, float v2);
	bool SetUniform4f(const string& name, float v0, float v1, float v2, float v3);
	bool SetUniformMat4f(const string& name, const glm::mat4& mat);
    

    void Bind();
    void Unbind();

private:
    
    bool SetShaders(std::unordered_map<ShaderType,std::vector<std::string>> source);
    bool CompileShader(unsigned int shaderID,std::string type);
    bool LinkShader();


    std::map<std::string,ShaderUniformContainer> m_UniformLocations;


    bool m_AlreadyCreatedProgram = false;
    std::vector<unsigned int> m_CompiledShadersCache;
    std::shared_ptr<unsigned int> m_ID;



    friend struct ShaderCreationProperties;
};