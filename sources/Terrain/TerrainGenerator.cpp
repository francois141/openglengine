#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(Camera *camera_in, Light *light_in, MasterRenderer *renderer_in) {
    const string vertex_shader_path = BASE_PATH_SHADERS + "terrain_v.glsl";
    const string fragment_shader_path = BASE_PATH_SHADERS + "terrain_f.glsl";

    shader = new Shader(vertex_shader_path, fragment_shader_path);

    camera = camera_in;
    light = light_in;
    renderer = renderer_in;
    // Load height data
    int width, height, nrChannels;
    unsigned char *dataHeightMap = stbi_load("../sources/Terrain/Resources/heightmap.jpg", &width, &height, &nrChannels,
                                             0);
    heightData = vector<vector<double> >(height, vector<double>(width, 0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            heightData[i][j] = dataHeightMap[3 * (width * i + j)] / (double) 255 * 6.0 - 2.0;
        }
    }
    // generate terrain
    data = generateTerrain();

    texture1 = TextureLoader::loadTexture(BASE_PATH_RESOURCES + "texture1.png");
    texture2 = TextureLoader::loadTexture(BASE_PATH_RESOURCES + "texture2.png");
    texture3 = TextureLoader::loadTexture(BASE_PATH_RESOURCES + "texture5.png");
    texture4 = TextureLoader::loadTexture(BASE_PATH_RESOURCES + "texture2.png");

    blendMap = TextureLoader::loadTexture(BASE_PATH_RESOURCES + "blendmap.png");

    createBuffers();
    // Setup correct size
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2, 1));
}

void TerrainGenerator::draw() {
    if (data.size() <= 0)
        return;
    _draw(NORMAL);
}

void TerrainGenerator::drawShadow() {
    if (data.size() <= 0)
        return;
    _draw(SHADOW);
}

void TerrainGenerator::drawLowQuality() {
    if (data.size() <= 0)
        return;
    _draw(LOW);
}

void TerrainGenerator::_draw(const RenderType RENDER_TYPE) {
    bind();
    passToShader(RENDER_TYPE);
    glDrawArrays(GL_TRIANGLES, 0, data.size());
    unbind();
}

void TerrainGenerator::createBuffers() {
    // Create VAO
    glGenVertexArrays(1, &VAO);
    // Create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
}

void TerrainGenerator::remove() {
    // Delete VBO
    glDeleteBuffers(1, &VBO);
    // Delete VAO;
    glDeleteVertexArrays(1, &VAO);
}

void TerrainGenerator::bind() {
    // Bind VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Bind VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, blendMap);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, renderer->depthMapID);
}

void TerrainGenerator::passToShader(RenderType type) {
    shader->use();

    shader->passCameraUniformVariables(camera);
    shader->passLightUniformVariables(light);

    shader->setMat4("model", modelMatrix);

    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);
    shader->setInt("texture3", 2);
    shader->setInt("texture4", 3);
    shader->setInt("blendmap", 4);
    shader->setInt("ShadowMap", 5);

    shader->setInt("RenderType", type);
}

void TerrainGenerator::unbind() {
    // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Unbind Texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

vector<float> TerrainGenerator::generateTerrain() {
    vertexData = vector<float>(0);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            generateCell(i - SIZE / 2, j - SIZE / 2, i, j);
        }
    }
    return vertexData;
}

void TerrainGenerator::generateCell(int offsetX, int offsetZ, int x, int z) {

    unsigned int SIZE = 255;

    float step = 1 / (float) SIZE;

    glm::vec2 tCoords1 = glm::vec2(x * step, z * step);
    glm::vec2 tCoords2 = glm::vec2(x * step, (z + 1) * step);
    glm::vec2 tCoords3 = glm::vec2((x + 1) * step, z * step);
    glm::vec2 tCoords4 = glm::vec2((x + 1) * step, (z + 1) * step);

    glm::vec3 normal1;
    glm::vec3 normal2;
    glm::vec3 normal3;
    glm::vec3 normal4;

    if (x <= 1 || x >= 254 || z <= 1 || z >= 254) {
        normal1 = glm::vec3(0, 1, 0);
        normal2 = glm::vec3(0, 1, 0);
        normal3 = glm::vec3(0, 1, 0);
        normal4 = glm::vec3(0, 1, 0);
    } else {
        normal1 = glm::normalize(glm::vec3(heightData[x - 1][z] - heightData[x + 1][z], 2.0,
                                           heightData[x][z - 1] - heightData[x][z + 1]));
        normal2 = glm::normalize(glm::vec3(heightData[x - 1][z + 1] - heightData[x + 1][z + 1], 2.0,
                                           heightData[x][z] - heightData[x][z + 2]));
        normal3 = glm::normalize(glm::vec3(heightData[x][z] - heightData[x + 2][z], 2.0,
                                           heightData[x + 1][z - 1] - heightData[x + 1][z + 1]));
        normal4 = glm::normalize(glm::vec3(heightData[x][z + 1] - heightData[x + 2][z + 1], 2.0,
                                           heightData[x + 1][z] - heightData[x + 1][z + 2]));
    }

    constexpr glm::vec3 point1 = glm::vec3(0, 0, 0);
    constexpr glm::vec3 point2 = glm::vec3(0, 0, 1);
    constexpr glm::vec3 point3 = glm::vec3(1, 0, 0);
    constexpr glm::vec3 point4 = glm::vec3(1, 0, 1);
    constexpr glm::vec3 normal = glm::vec3(0, 1.0, 0);

    addVecOffset(point1, offsetX, offsetZ, x, z);
    addVec(normal1);
    addVec(tCoords1);
    addVecOffset(point2, offsetX, offsetZ, x, z + 1);
    addVec(normal2);
    addVec(tCoords2);
    addVecOffset(point3, offsetX, offsetZ, x + 1, z);
    addVec(normal3);
    addVec(tCoords3);

    addVecOffset(point3, offsetX, offsetZ, x + 1, z);
    addVec(normal3);
    addVec(tCoords3);
    addVecOffset(point2, offsetX, offsetZ, x, z + 1);
    addVec(normal2);
    addVec(tCoords2);
    addVecOffset(point4, offsetX, offsetZ, x + 1, z + 1);
    addVec(normal4);
    addVec(tCoords4);
}

void TerrainGenerator::addVecOffset(glm::vec3 data, int offsetX, int offsetZ, int x, int z) {
    vertexData.push_back(data.x + offsetX);
    vertexData.push_back(heightData[x][z]);
    vertexData.push_back(data.z + offsetZ);
}

void TerrainGenerator::addVec(glm::vec2 data) {
    vertexData.push_back(data.x);
    vertexData.push_back(data.y);
}

void TerrainGenerator::addVec(glm::vec3 data) {
    vertexData.push_back(data.x);
    vertexData.push_back(data.y);
    vertexData.push_back(data.z);
}