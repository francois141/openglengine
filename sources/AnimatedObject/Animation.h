#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimatedObject/Bone.h"
#include "AnimatedObject/AnimatedObject.h"

#include <map>

struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class Animation
{
public:
    Animation() = default;
    Animation(const std::string &animationPath, AnimatedObject *model);
    ~Animation();
    Bone *FindBone(const std::string &name);

    inline float GetTickPerSeconds() { return m_TicksPerSecond; }

    inline float GetDuration() { return m_Duration; }

    inline const AssimpNodeData &GetRootNode() { return m_RootNode; }

    inline const std::map<std::string, BoneInfo> &GetBoneIdMap()
    {
        return m_BoneInfoMap;
    }

private:
    void ReadMissingBones(const aiAnimation *animation, AnimatedObject &model);
    void ReadHeirarchyData(AssimpNodeData &dest, const aiNode *src);
    
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};

#endif