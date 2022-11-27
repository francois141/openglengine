#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "AnimatedObject/Animation.h"

class Animator
{
public:
    Animator(Animation *Animation);
    void UpdateAnimation(float dt);
    void PlayAnimation(Animation *pAnimation);
    void CalculateBoneTransform(const AssimpNodeData *node, glm::mat4 parentTransform);
    std::vector<glm::mat4> GetFinalBoneMatrices();

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation *m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};

#endif