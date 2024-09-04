#include "Joint.h"
#include <Matrix4x4Calculation.h>

int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints){
    Joint joint = {};
    joint.name = node.name;
    joint.localMatrix = node.localMatrix;
    joint.skeletonSpaceMatrix = MakeIdentity4x4();
    joint.transform = node.transform;
    //現在登録されている数をIndexに
    joint.index = int32_t(joints.size());
    joint.parent = parent;
    //SkeletonのJoint列に追加
    joints.push_back(joint);
    for (const Node& child : node.children) {
        //子Jointを作成し、そのIndexを登録
        int32_t childIndex = CreateJoint(child, joint.index, joints);
        joints[joint.index].cheldren.push_back(childIndex);
    }


    //自身のIndexを返す
    return joint.index;
}
