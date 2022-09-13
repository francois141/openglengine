#include "Shader.h"

Shader::Shader() {

}

Shader::Shader(string vertexPath, string fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    vertexCode = addIncludes(vertexCode);
    fragmentCode = addIncludes(fragmentCode);

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const string &name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::passCameraUniformVariables(Camera *camera) {
    setMat4("projection", camera->getProjectionMatrix());
    setMat4("view", camera->getCamera());
    setVec3("cameraPosition", camera->getCameraPosition());
    setMat4("lightSpaceMatrix", camera->getShadowCamera());
}

void Shader::passLightUniformVariables(Light *light) {
    setVec3("light.lightDirection", light->getDirection());

    for (int i = 0; i < light->getPositions().size(); i++) {
        setVec3("pointLights[" + to_string(i) + "].position", light->getPositions()[i]);
        setVec3("pointLights[" + to_string(i) + "].color", light->getColors()[i]);
    }
}

void Shader::passModelMatrix(glm::mat4 modelMatrix) {
    setMat4("model", modelMatrix);
}


void Shader::passMaterial(Material mapInfo) {
    setInt("ShadowMap", 0);
    setInt("albedoMap", 1);
    setInt("specularMap", 2);
    setInt("normalMap", 3);

    setVec3("ambientColor", mapInfo.ambientColor);
    setVec3("diffuseColor", mapInfo.diffuseColor);
    setVec3("specularColor", mapInfo.specularColor);

    setFloat("specularExponent", mapInfo.specularExponent);

    setBool("hasAlbedoMap", mapInfo.hasAlbedo);
    setBool("hasNormalMap", mapInfo.hasNormal);
    setBool("hasSpecularMap", mapInfo.hasSpecular);
}

string Shader::addIncludes(std::string code) {

    auto extract_path = [](const string matched_include) -> string {

        auto iteratorLeft = std::find(matched_include.begin(), matched_include.end(), '(');
        auto iteratorRight = std::find(matched_include.begin(), matched_include.end(), ')');

        const int start = std::distance(matched_include.begin(), iteratorLeft) + 1;
        const int end = std::distance(matched_include.begin(), iteratorRight) - start;

        return matched_include.substr(start,end);
    };

    auto format_regex = [](string include_string) -> regex {

        auto iteratorLeft = std::find(include_string.begin(), include_string.end(), '(');
        const int distanceLeft = std::distance(include_string.begin(), iteratorLeft);
        include_string.insert(distanceLeft,"\\");

        auto iteratorRight = std::find(include_string.begin(), include_string.end(), ')');
        const int distanceRight = std::distance(include_string.begin(), iteratorRight);
        include_string.insert(distanceRight,"\\");

        return regex(include_string);
    };
    
    // Iterate until we are done. So we can include submodules.
    // If the number of includes is bigger than MAX_INCLUDE_FILES, then we can assume we have a cycle and we should break it

    unsigned int count = 0;
    unsigned int prevCount = -1;

    while(count <= MAX_INCLUDE_FILES_DEPTH && count != prevCount) {

        const regex matcher = regex(MODULE_REGEX_MATCHER);
        smatch m;
        map<string,string> fragments;

        prevCount = count;

        string::const_iterator searchStart(code.cbegin());
        while (regex_search(searchStart,code.cend(),m,matcher)) {
            
            for (string matched_include: m) {
                const string path = extract_path(matched_include);
                const string content = readFile(path);
                fragments.insert({matched_include,content});
            }

            searchStart = m.suffix().first;
        } 

        for (const pair<string,string> &pair : fragments ) {
            const regex expr = format_regex(pair.first);
            code = regex_replace(code,expr,pair.second);
        }

        count += 1 - fragments.empty();
    }

    return count > MAX_INCLUDE_FILES_DEPTH ? "" : code;
}

string Shader::readFile(std::string path) {

    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {

        file.open(path);
        std::stringstream codeStream;
        codeStream << file.rdbuf();

        file.close();
        // convert stream into string
        code = codeStream.str();
    }
    catch (std::ifstream::failure &e) {
        // TODO: Add logger here
        std::cout << "ERROR::SHADER::MODULE_NOT_SUCCESFULLY_READ" << std::endl;
        exit(1);
    }

    return code;
}


void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

