#include "Skeleton.h"
#include <Matrix4x4Calculation.h>

void Skeleton::Create(const Node& rootNode){
    root_ = CreateJoint(rootNode, {}, joints_);

    
    for (const Joint& joint : joints_) {
      
        jointMap_.emplace(joint.name, joint.index);
    }

}

void Skeleton::Update(){
   
    for (Joint& joint : joints_) {

        Matrix4x4 scaleMatrix = MakeScaleMatrix(joint.transform.scale);
        Matrix4x4 rotateMatrix = MakeRotateMatrix(joint.transform.rotate);
        Matrix4x4 translateMatrix = MakeTranslateMatrix(joint.transform.translate);
        

        joint.localMatrix = Multiply(scaleMatrix,Multiply(rotateMatrix, translateMatrix));

        if (joint.parent) {
            joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, joints_[*joint.parent].skeletonSpaceMatrix);
        }
        
        else {
            joint.skeletonSpaceMatrix = joint.localMatrix;
        }

    }


}
