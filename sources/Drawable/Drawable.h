#ifndef DRAWABLE_H
#define DRAWABLE_H

#define PURE_VIRTUAL = 0 // this is just a cleaner way to say that a function is pure virtual

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shaders/Shader.h"
#include "Core/Light.h"
#include "Core/Camera.h"

class MasterRenderer;

class Drawable {

    public:
        // Simple draw functions
        virtual void draw() PURE_VIRTUAL;
        virtual void drawShadow() PURE_VIRTUAL;
        virtual void drawLowQuality() PURE_VIRTUAL;
        // Functions to manage the position of the drawable
        void setPosition(glm::vec3);
        glm::vec3 getPosition();
        // Functions to manage the rotation of the drawable
        void rotateX(float degree);
        void rotateY(float degree);
        void rotateZ(float degree);
        // Functions to manage the scale of the drawable
        void setScale(glm::vec3);
        glm::vec3 getScale();

    protected:
        // Some necessary variables to work
        MasterRenderer *renderer;
        Light          *light;
        Camera         *camera;

        Shader *shader;
        Shader *shader_shadow;
        Shader *shader_lowquality;

        unsigned int VBO;
        unsigned int VAO;
        unsigned int textureID;

        glm::vec3 position = glm::vec3(0,0,0);
        glm::vec3 rotation = glm::vec3(0,0,0);
        glm::mat4 modelMatrix = glm::mat4(1.0f); 
        glm::vec3 scale = glm::vec3(1,1,1);

        float transparency = 1.0;

        float shadowDistance = 50;

        vector<float> data; 
            
};

#endif
