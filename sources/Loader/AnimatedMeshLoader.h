#ifndef ANIMATEDMESHLOADER_H
#define ANIMATEDMESHLOADER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Loader/AssimpGLMHelper.hpp"

#include "Loader/TextureLoader.h"


#define MAX_BONE_INFLUENCE 4

struct AnimatedVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    glm::vec2 TexCoords;

    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct AnimatedTexture
{
    unsigned int id;
    std::string path;
    std::string type;
};

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

class AnimatedMeshLoader
{
public:

    AnimatedMeshLoader() {}
    AnimatedMeshLoader(std::string path_in) : path(path_in)
    {
        indices = std::vector<std::vector<unsigned int>>(0, std::vector<unsigned int>(0));
        data = std::vector<std::vector<AnimatedVertex>>(0, std::vector<AnimatedVertex>(0));

        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP:" << import.GetErrorString() << std::endl;
        }
        else
        {
            directory = path.substr(0, path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }
    }

    std::vector<AnimatedVertex> getData(int i)
    {
        return data.at(i);
    }

    std::vector<unsigned int> getIndices(int i)
    {
        return indices.at(i);
    }

    std::vector<AnimatedTexture> getTextures() {
        return textures;
    }

    int getNbMeshes()
    {
        return nbMeshes;
    }

    std::map<std::string,BoneInfo> m_BoneInfoMap;    
    std::string path;
    std::string directory;
    std::vector<std::vector<AnimatedVertex>> data;
    std::vector<std::vector<unsigned int>> indices;
    std::vector<AnimatedTexture> textures;
    std::vector<AnimatedTexture> textures_loaded;
    int m_BoneCounter;
    int nbMeshes = 0;

private:


    void SetVertexBoneDataToDefault(AnimatedVertex& vertex) {
        for(int i = 0; i < 4;i++) {
            vertex.m_BoneIDs[i] = -1;
            vertex.m_Weights[i] = 0.0f;
        }
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            auto output = processMesh(mesh, scene);
            data.push_back(output.first);
            indices.push_back(output.second);
            nbMeshes++;
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    std::pair<std::vector<AnimatedVertex>, std::vector<unsigned int>> processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::vector<AnimatedVertex> data_current = std::vector<AnimatedVertex>(0);
        std::vector<unsigned int> indices_current = std::vector<unsigned int>(0);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 textureCoord;
            glm::vec3 tangent; 
            glm::vec3 bitangent;

            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;

            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;

            tangent.x = mesh->mTangents[i].x;
            tangent.y = mesh->mTangents[i].y;
            tangent.z = mesh->mTangents[i].z;

            bitangent.x = mesh->mBitangents[i].x;
            bitangent.y = mesh->mBitangents[i].y;
            bitangent.z = mesh->mBitangents[i].z;
            
            textureCoord.x = mesh->mTextureCoords[0][i].x;
            textureCoord.y = 1 - mesh->mTextureCoords[0][i].y;

            AnimatedVertex currentVertex = AnimatedVertex();
            currentVertex.Position = position;
            currentVertex.Normal = normal;
            currentVertex.TexCoords = textureCoord;
            currentVertex.Tangent = tangent;
            currentVertex.Bitangent = bitangent;

            SetVertexBoneDataToDefault(currentVertex);
            data_current.push_back(currentVertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices_current.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<AnimatedTexture> diffuseMaps = loadMaterialTextures(material,
                                                               aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<AnimatedTexture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<AnimatedTexture> normalMaps = loadMaterialTextures(material,
                                                                aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(),normalMaps.begin(),normalMaps.end());
        }

        ExtractBoneWeightForVertices(data_current,mesh,scene);

        return make_pair(data_current, indices_current);
    }

    void ExtractBoneWeightForVertices(std::vector<AnimatedVertex>& vertices, aiMesh* mesh, const aiScene* scene){
        for(int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
            int boneID = -1;
            std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
            if(m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {
                BoneInfo newBoneInfo;
                newBoneInfo.id = m_BoneCounter;
                newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                m_BoneInfoMap[boneName] = newBoneInfo;
                boneID = m_BoneCounter;
                m_BoneCounter++;
            }
            else {
                boneID = m_BoneInfoMap[boneName].id;
            }
            auto weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            for(int weightIndex = 0; weightIndex < numWeights;weightIndex++) {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                SetVertexBoneData(vertices[vertexId],boneID,weight);
            }
        }

    }

    void SetVertexBoneData(AnimatedVertex& vertex, int boneID,float weight) {
        for(int i = 0; i < MAX_BONE_INFLUENCE;i++)
        {
            if(vertex.m_BoneIDs[i] < 0){
                vertex.m_Weights[i] = weight;
                vertex.m_BoneIDs[i] = boneID;
                break;
            }
        }
    }



    std::vector<AnimatedTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<AnimatedTexture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            AnimatedTexture texture;
            bool skip = false;
            for(int i = 0; i < textures_loaded.size();i++) {
                if(textures_loaded[i].path == (directory + "/" + str.C_Str()))
                {
                    skip = true;
                }
            }
            if(!skip) {
                TextureLoader loader = TextureLoader( directory + "/" + str.C_Str());
                texture.id = loader.loadTexture();
                texture.path = directory + "/" + str.C_Str();
                texture.type = typeName;
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
};

#endif