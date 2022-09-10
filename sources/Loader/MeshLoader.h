#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Loader/TextureLoader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string path;
    std::string type;
};

class MeshLoader {
public:
    MeshLoader(std::string path_in) : path(path_in) {
        indices = std::vector<std::vector<unsigned int>>(0, std::vector<unsigned int>(0));
        data = std::vector<std::vector<float>>(0, std::vector<float>(0));
        materials = std::vector<Material>(0);

        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path,
                                               aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP:" << import.GetErrorString() << std::endl;
        } else {
            directory = path.substr(0, path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }
    }

    std::vector<float> getData(int i) {
        return data.at(i);
    }

    std::vector<unsigned int> getIndices(int i) {
        return indices.at(i);
    }

    std::vector<Texture> getTextures() {
        return textures;
    }

    int getNbMeshes() {
        return nbMeshes;
    }

    Material getMaterial(int i) {
        return materials.at(i);
    }

    glm::vec3 getAmbientColor() {
        return ambientColor;
    }

private:
    std::string path;
    std::string directory;
    std::vector<vector<float>> data;
    std::vector<vector<unsigned int>> indices;
    std::vector<Material> materials;
    std::vector<Texture> textures;
    std::vector<Texture> textures_loaded;

    unsigned int nbMeshes = 0;


    glm::vec3 ambientColor = glm::vec3(0, 0, 1);

    void processNode(aiNode *node, const aiScene *scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            auto output = processMesh(mesh, scene);
            data.push_back(output.first);
            indices.push_back(output.second);
            nbMeshes++;
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    std::pair<std::vector<float>, std::vector<unsigned int>> processMesh(aiMesh *mesh, const aiScene *scene) {
        vector<float> data_current = vector<float>(0);
        vector<unsigned int> indices_current = vector<unsigned int>(0);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            data_current.push_back(mesh->mVertices[i].x);
            data_current.push_back(mesh->mVertices[i].y);
            data_current.push_back(mesh->mVertices[i].z);

            data_current.push_back(mesh->mNormals[i].x);
            data_current.push_back(mesh->mNormals[i].y);
            data_current.push_back(mesh->mNormals[i].z);

            data_current.push_back(mesh->mTangents[i].x);
            data_current.push_back(mesh->mTangents[i].y);
            data_current.push_back(mesh->mTangents[i].z);

            data_current.push_back(mesh->mBitangents[i].x);
            data_current.push_back(mesh->mBitangents[i].y);
            data_current.push_back(mesh->mBitangents[i].z);

            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                data_current.push_back(mesh->mTextureCoords[0][i].x);
                data_current.push_back(1 - mesh->mTextureCoords[0][i].y);
            } else {
                data_current.push_back(mesh->mTextureCoords[0][i].x);
                data_current.push_back(mesh->mTextureCoords[0][i].y);
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices_current.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                               aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            vector<Texture> normalMaps = loadMaterialTextures(material,
                                                              aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *materialData = scene->mMaterials[mesh->mMaterialIndex];
            Material material = Material();

            aiColor3D color_ambient(0.f, 0.f, 0.f);
            aiColor3D color_diffuse(0.f, 0.f, 0.f);
            aiColor3D color_specular(0.f, 0.f, 0.f);

            float specularExponent = 0.0f;

            materialData->Get(AI_MATKEY_COLOR_AMBIENT, color_ambient);
            materialData->Get(AI_MATKEY_COLOR_DIFFUSE, color_diffuse);
            materialData->Get(AI_MATKEY_COLOR_SPECULAR, color_specular);
            materialData->Get(AI_MATKEY_SHININESS, specularExponent);

            material.ambientColor = convertToVector(color_ambient);
            material.diffuseColor = convertToVector(color_diffuse);
            material.specularColor = convertToVector(color_specular);
            material.specularExponent = specularExponent;

            materials.push_back(material);
        }

        return make_pair(data_current, indices_current);
    }

    inline glm::vec3 convertToVector(aiColor3D input) {
        glm::vec3 output;
        output[0] = input[0];
        output[1] = input[1];
        output[2] = input[2];
        return output;
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            Texture texture;
            bool skip = false;
            for (int i = 0; i < textures_loaded.size(); i++) {
                if (textures_loaded[i].path == (directory + "/" + str.C_Str())) {
                    //textures.push_back(textures_loaded[i]);
                    skip = true;
                }
            }
            if (!skip) {
                const string texture_path = directory + "/" + str.C_Str();
                texture.id = TextureLoader::loadTexture(texture_path);
                texture.path = texture_path;
                texture.type = typeName;
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
};

#endif