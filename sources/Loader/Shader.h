#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Camera.h"
#include "Core/Light.h"
#include "Core/Material.h"

class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader()
	{

	}

	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
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
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
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

	Shader(const char* vertexPath, const char* fragmentPath,bool Include)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string includeCodeVertex;
		std::string includeCodeFragment;
		std::string functionCodeVertex;
		std::string functionCodeFragment;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream includeShaderFileVertex;
		std::ifstream includeShaderFileFragment;
		std::ifstream functionShaderFileVertex;
		std::ifstream functionShaderFileFragment;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		includeShaderFileVertex.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		includeShaderFileFragment.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		functionShaderFileVertex.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		functionShaderFileFragment.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			includeShaderFileVertex.open("../sources/ShaderClassic/IncludeVertex.glsl");
			includeShaderFileFragment.open("../sources/ShaderClassic/IncludeFragment.glsl");
			functionShaderFileVertex.open("../sources/ShaderClassic/ClassicFunctionsVertex.glsl");
			functionShaderFileFragment.open("../sources/ShaderClassic/ClassicFunctionsFragment.glsl");
			std::stringstream vShaderStream, fShaderStream,includeShaderStreamFragment;
			std::stringstream includeShaderStreamVertex,functionShaderStreamVertex,functionShaderStreamFragment;

			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			includeShaderStreamVertex << includeShaderFileVertex.rdbuf();
			includeShaderStreamFragment << includeShaderFileFragment.rdbuf();
			functionShaderStreamVertex << functionShaderFileVertex.rdbuf();
			functionShaderStreamFragment << functionShaderFileFragment.rdbuf();

			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			includeShaderFileVertex.close();
			includeShaderFileFragment.close();
			functionShaderFileVertex.close();
			functionShaderFileFragment.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			includeCodeVertex = includeShaderStreamVertex.str();
			includeCodeFragment = includeShaderStreamFragment.str();
			functionCodeVertex = functionShaderStreamVertex.str();
			functionCodeFragment = functionShaderStreamFragment.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// Add include code to the vertex shader
		vertexCode = includeCodeVertex + "\n" + vertexCode + "\n" + functionCodeVertex;
		// Add include code to the fragment shader 
		fragmentCode = includeCodeFragment + "\n" + fragmentCode + "\n" + functionCodeFragment;

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
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
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void passCameraUniformVariables(Camera *camera)
	{
		setMat4("projection", camera->getProjectionMatrix());
		setMat4("view", camera->getCamera());
		setVec3("cameraPosition", camera->getCameraPosition());
		setMat4("lightSpaceMatrix",camera->getShadowCamera());
	}

	void passLightUniformVariables(Light *light)
	{		
		setVec3("light.lightDirection", light->getDirection());

		for (int i = 0; i < light->getPositions().size();i++)
		{
			setVec3("pointLights[" + to_string(i) + "].position", light->getPositions()[i]);
			setVec3("pointLights[" + to_string(i) + "].color", light->getColors()[i]);
		}
	}
	
	void passModelMatrix(glm::mat4 modelMatrix) 
	{
		setMat4("model",modelMatrix);
	}
	
	void passMaterial(Material mapInfo) {
		setInt("ShadowMap", 0);	
		setInt("albedoMap", 1);
		setInt("specularMap", 2);
		setInt("normalMap", 3);

		setVec3("ambientColor",mapInfo.ambientColor);
		setVec3("diffuseColor",mapInfo.diffuseColor);
		setVec3("specularColor",mapInfo.specularColor);

		setFloat("specularExponent",mapInfo.specularExponent);

		setBool("hasAlbedoMap",mapInfo.hasAlbedo);
		setBool("hasNormalMap",mapInfo.hasNormal);
		setBool("hasSpecularMap",mapInfo.hasSpecular);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif