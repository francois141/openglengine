#include "Object.h"

Object::Object(Camera *camera_in, Light *light_in, string obj_path, MasterRenderer *renderer_in) {
    shader = new Shader("../sources/Object/Shaders/object_v.glsl", "../sources/Object/Shaders/object_f.glsl", true);

    renderer = renderer_in;
    camera = camera_in;
    light = light_in;

    MeshLoader loader = MeshLoader(obj_path);
    nbMeshes = loader.getNbMeshes();

    for (int i = 0; i < nbMeshes; i++) {
        meshData.push_back(loader.getData(i));
        meshIndices.push_back(loader.getIndices(i));
        materials.push_back(loader.getMaterial(i));
        createBuffers(i);
    }

    vector<Texture> textures = loader.getTextures();
    for (int i = 0; i < textures.size(); i++) {

        string textureType = textures.at(i).type;

        if (textures.at(i).type == "texture_diffuse") {
            textureID = textures.at(i).id;
            materials[0].hasAlbedo = true;
            materials[0].albedoTextureID = textures.at(i).id;
        }
        if (textures.at(i).type == "texture_specular") {
            specularTexture = textures.at(i).id;
            materials[0].hasSpecular = true;
            materials[0].specularTextureID = textures.at(i).id;
        }
        if (textures.at(i).type == "texture_normal") {
            normalMap = textures.at(i).id;
            materials[0].hasNormal = true;
            materials[0].normalTextureID = textures.at(i).id;
        }
    }
}

void Object::draw() {

    if (nbMeshes == 0)
        return;

    this->_draw(NORMAL);
}

void Object::drawShadow() {
    if (nbMeshes == 0)
        return;

    this->_draw(SHADOW);
}

void Object::drawLowQuality() {
    if (nbMeshes == 0)
        return;

    this->_draw(LOW);
}

void Object::_draw(RenderType TYPE) {
    for (int i = 0; i < nbMeshes; i++) {
        this->bind(i);
        this->passToShader(TYPE);
        glDrawElements(GL_TRIANGLES, meshIndices[i].size(), GL_UNSIGNED_INT, (void *) 0);
    }
    unbind();
}


void Object::createBuffers(int index) {
    unsigned int VAO, VBO, EBO;
    // Create VAO
    glGenVertexArrays(1, &VAO);
    // Create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * meshData[index].size(), &meshData[index][0], GL_STATIC_DRAW);
    // Create EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices[index].size() * sizeof(unsigned int), &meshIndices[index][0],
                 GL_STATIC_DRAW);

    VAOS.push_back(VAO);
    VBOS.push_back(VBO);
    EBOS.push_back(EBO);
}

void Object::bind(int index) {

    // Bind VAO
    glBindVertexArray(VAOS[index]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[index]);

    // Bind VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (12 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) (9 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS[index]);

    // Bind texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderer->depthMapID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalMap);
}

void Object::passToShader(RenderType type) {

    shader->use();

    shader->passCameraUniformVariables(camera);
    shader->passLightUniformVariables(light);
    shader->passModelMatrix(modelMatrix);
    shader->passMaterial(materials[0]);

    shader->setInt("RenderType", type);
}

void Object::unbind() {

    // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Unbind Texture
    glBindTexture(GL_TEXTURE_2D, 0);
}