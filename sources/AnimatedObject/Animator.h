#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "AnimatedObject/Animation.h"

class Animator 
{
    public:
    Animator(Animation* Animation)
    {
        m_CurrentTime = 0.0;
        m_CurrentAnimation = Animation;

        m_FinalBoneMatrices.reserve(100);

        for(int i = 0; i < 100;i++)
        {
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        }
    }

    void UpdateAnimation(float dt)
    {
        if(m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTickPerSeconds() * dt;
            m_CurrentTime = fmod(m_CurrentTime,m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(),glm::mat4(1.0f));
        }
    }

    void PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_CurrentTime = 0.0f;
    }

    void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;

       

        Bone *bone = m_CurrentAnimation->FindBone(nodeName);

        if(bone)
        {
            bone->Update(m_CurrentTime);
            nodeTransform = bone->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;


        auto boneInfoMap = m_CurrentAnimation->GetBoneIdMap();
        if(boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation *  offset;
        }

        for(int i = 0; i < node->childrenCount; i++)
        {
            CalculateBoneTransform(&node->children[i],globalTransformation);
        }
    }

    std::vector<glm::mat4> GetFinalBoneMatrices()
    {
        return m_FinalBoneMatrices;
    }

    private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;	
};

#endif