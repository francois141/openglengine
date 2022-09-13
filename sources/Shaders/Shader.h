#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Core/Camera.h"
#include "Core/Light.h"
#include "Core/Material.h"

class Shader {
public:
    unsigned int ID;

    // The constructor generates the shaders
    Shader();
    Shader(string vertexPath, string fragmentPath);

    // This function activate the shader
    void use();

    // These functions pass uniform to the shaders
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void passCameraUniformVariables(Camera *camera);
    void passLightUniformVariables(Light *light);
    void passModelMatrix(glm::mat4 modelMatrix);
    void passMaterial(Material mapInfo);

private:
    // Parse glsl code add include dependencies
    string addIncludes(string code);
    string readFile(string path);

    std::string MODULE_REGEX_MATCHER = "ADD_MODULE\\(.*\\)";
    static const unsigned int MAX_INCLUDE_FILES_DEPTH = 5;

    // Utility function
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif