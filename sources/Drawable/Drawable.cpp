#include "Drawable.h"

void Drawable::setPosition(glm::vec3 position_in) {
    modelMatrix = glm::translate(glm::mat4(1.0f),position_in);
    modelMatrix = glm::scale(modelMatrix,scale);
    position    = position_in;
}

glm::vec3 Drawable::getPosition() {
    return position;
}

void Drawable::rotateX(float degree) {
    rotation.x += degree;
    modelMatrix = glm::rotate(modelMatrix,glm::radians(degree),glm::vec3(1,0,0));
}

void Drawable::rotateY(float degree) {
    rotation.y += degree;
    modelMatrix = glm::rotate(modelMatrix,glm::radians(degree),glm::vec3(0,1,0));
}

void Drawable::rotateZ(float degree) {
    rotation.z += degree;
    modelMatrix = glm::rotate(modelMatrix,glm::radians(degree),glm::vec3(0,0,1));
}

void Drawable::setScale(glm::vec3 scale_in) {
    modelMatrix = glm::translate(modelMatrix,position);
    modelMatrix = glm::scale(glm::mat4(1.0f),scale_in);
    scale = scale_in;
}


glm::vec3 Drawable::getScale() {
    return scale;
}