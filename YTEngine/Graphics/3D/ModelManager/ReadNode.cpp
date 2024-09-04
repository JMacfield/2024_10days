#include "ReadNode.h"
#include <Matrix4x4Calculation.h>

ReadNode* ReadNode::GetInstance(){
    static ReadNode instance;

    return &instance;
}

Node ReadNode::Read(aiNode* node){
    Node result = {};

  




    aiVector3D scale = {};
    aiVector3D translate = {};
    aiQuaternion rotate = {};
    //assimpの行列からSRTを抽出する関数を利用
    node->mTransformation.Decompose(scale, rotate, translate);
    //Scale
    result.transform.scale = { scale.x,scale.y,scale.z };
    //rotate
    result.transform.rotate = { rotate.x,-rotate.y,-rotate.z,rotate.w };
    //translate
    result.transform.translate = { -translate.x,translate.y,translate.z};

    Vector3 newRotate = { result.transform.rotate.x, result.transform.rotate.y, result.transform.rotate.z };
    result.localMatrix = MakeAffineMatrix(result.transform.scale, newRotate, result.transform.translate);


    //Node名を格納
    result.name = node->mName.C_Str();
    //子供の数だけ確保
    result.children.resize(node->mNumChildren);
    for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
        //再帰的に読んで階層構造を作っていく
        result.children[childIndex] = Read(node->mChildren[childIndex]);
    }

    return result;
}

