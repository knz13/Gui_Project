#include "shader.h"




Shader::Shader() {
    


}



void Shader::Bind() {
    if(!m_ID){
        return;
    }
    GL_CALL(glUseProgram(*m_ID.get()));
}

void Shader::Unbind() {
    GL_CALL(glUseProgram(0));
}



bool Shader::SetShaders(std::unordered_map<ShaderType,std::vector<std::string>> sources) {
    if(m_AlreadyCreatedProgram){
        m_ID.reset();
        m_AlreadyCreatedProgram = false;
    }
    
    if(sources.size() == 0){
        DEBUG_LOG("No sources were provided so shader.");
        return false;
    }

    for(auto& [type,sources] : sources){
        unsigned int shaderID = -1;
        
        std::string shaderTypeName;
        switch(type){
        case ShaderType::Vertex:
            shaderTypeName = "Vertex";
            break;
        case ShaderType::Fragment:
            shaderTypeName = "Fragment";
            break;
            
        }

        switch(type){
        case ShaderType::Vertex:
            GL_CALL(shaderID = glCreateShader(GL_VERTEX_SHADER));
            break;
        case ShaderType::Fragment:
            GL_CALL(shaderID = glCreateShader(GL_FRAGMENT_SHADER)); 
            break;
        }

        std::vector<GLchar const*> files(sources.size());
        std::vector<GLint> lengths(sources.size());

        int index = 0;
        for(auto& source : sources){
            files[index] = source.c_str();
            lengths[index] = source.size();

            index++;
        }

        GL_CALL(glShaderSource(shaderID,index,files.data(),lengths.data()));

        if(!CompileShader(shaderID,shaderTypeName)){
            return false;
        } 
        m_CompiledShadersCache.push_back(shaderID);
    }

    if(!LinkShader()){
        return false;
    };

    m_CompiledShadersCache.clear();

    return true;

}
bool Shader::CompileShader(unsigned int shaderID,std::string shaderTypeName) {
    GL_CALL(glCompileShader(shaderID));

    GLint success = 0;
    GL_CALL(glGetShaderiv(shaderID,GL_COMPILE_STATUS,&success));

    if(success == GL_FALSE){
        GLint logsize = 0;
        GL_CALL(glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&logsize));

        std::vector<GLchar> errorLog(logsize);
        GL_CALL(glGetShaderInfoLog(shaderID,logsize,&logsize,&errorLog[0]));

        std::string errorStr;
        for(auto& character : errorLog){
            errorStr += character;
        }

        DEBUG_LOG("Shader compilation error: " << errorStr << " at shader of type " + shaderTypeName);

        GL_CALL(glDeleteShader(shaderID));

        return false;

    }
    

    if(!m_ID){
        auto deleter = [](unsigned int* id){
            GL_CALL(glDeleteProgram(*id));
        };

        m_ID = std::shared_ptr<unsigned int>(new unsigned int(0),deleter);
    }
    if(!m_AlreadyCreatedProgram){

        GL_CALL(*m_ID.get() = glCreateProgram());
        m_AlreadyCreatedProgram = true;
    }

    GL_CALL(glAttachShader(*m_ID.get(),shaderID));

    return true;

}

bool Shader::LinkShader() {
    GL_CALL(glLinkProgram(*m_ID.get()));

    int isLinked = 0;
    GL_CALL(glGetProgramiv(*m_ID.get(),GL_LINK_STATUS,&isLinked));

    if(isLinked == GL_FALSE){
        GLint maxLength = 0;
        glGetProgramiv(*m_ID.get(), GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(*m_ID.get(), maxLength, &maxLength, &infoLog[0]);
        
        std::string errorStr;
        for(auto& character : infoLog){
            errorStr += character;
        }

        DEBUG_LOG("Shader linking error: " << errorStr);
        
        
        
        for(auto id : m_CompiledShadersCache){
            GL_CALL(glDetachShader(*m_ID.get(),id));
            GL_CALL(glDeleteShader(id));
        }

        m_ID.reset();
        m_CompiledShadersCache.clear();
        
        return false;
        
    }

    // loading the uniforms

    int count;
    GL_CALL(glGetProgramiv(*m_ID.get(),GL_ACTIVE_UNIFORMS,&count));

    for(int i = 0;i<count;i++){

        std::vector<char> maxSize(255);
        std::string name = "";
        int length,size,type,location;
        GL_CALL(glGetActiveUniform(*m_ID.get(),i,maxSize.size(),&length,&size,(GLenum*)&type,maxSize.data()));
        
        for(int i = 0;i<length;i++){
            name += maxSize[i];
        }
        
        GL_CALL(location = glGetUniformLocation(*m_ID.get(),name.c_str()));


        ShaderUniformContainer container;
        container.location = location;
        container.size = size;
        container.type = type;
        m_UniformLocations[name] = std::move(container);
    }



    return true;

}

ShaderCreationProperties& ShaderCreationProperties::AddShader(ShaderType type, std::string source) {
    
    if(m_Shaders.find(type) == m_Shaders.end()){
        m_Shaders[type] = {source};
        return *this;
    }

    m_Shaders[type].emplace_back(std::move(source));
    return *this;
}

ShaderCreationProperties Shader::CreateNew() {
    return ShaderCreationProperties(*this);
}   

bool ShaderCreationProperties::Generate() {
    return m_Master->SetShaders(m_Shaders);
}

ShaderCreationProperties::ShaderCreationProperties(Shader& master) {
    m_Master = &master;
}

bool Shader::SetUniform1f(const string& name, float value) {
    this->Bind();
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        GL_CALL(glUniform1f(m_UniformLocations[name].location,value));
        return true;
    }
    else{
        return false;
    }
    
}

bool Shader::SetUniform1i(const string& name, int value) {
    this->Bind();
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        GL_CALL(glUniform1i(m_UniformLocations[name].location,value));
        return true;
    }
    else{
        return false;
    }
    
    
}

bool Shader::SetUniform3f(const string& name, float v0, float v1, float v2) {
    this->Bind();
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        GL_CALL(glUniform3f(m_UniformLocations[name].location,v0,v1,v2));
        return true;
    }
    else{
        return false;
    }
    
    
}

bool Shader::SetUniform4f(const string& name, float v0, float v1, float v2, float v3) {
    this->Bind();
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        GL_CALL(glUniform4f(m_UniformLocations[name].location,v0,v1,v2,v3));
        return true;
    }
    else{
        return false;
    }
    
}

bool Shader::SetUniformMat4f(const string& name, const glm::mat4& mat) {
    this->Bind();
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        GL_CALL(glUniformMatrix4fv(m_UniformLocations[name].location,1,GL_FALSE,glm::value_ptr(mat)));
        return true;
    }
    else{
        return false;
    }
    
}
bool Shader::SetUniform2f(const string& name, float v0, float v1) {
    this->Bind();
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        GL_CALL(glUniform2f(m_UniformLocations[name].location,v0,v1));
        return true;
    }
    else{
        return false;
    }
}

const std::map<std::string,ShaderUniformContainer>& Shader::GetUniformLocations() {
    return m_UniformLocations;
}
