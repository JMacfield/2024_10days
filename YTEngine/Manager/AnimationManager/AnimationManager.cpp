#include "AnimationManager.h"

#include <cassert>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <VectorCalculation.h>
#include "ModelManager.h"


uint32_t AnimationManager::handle_ = 0;


AnimationManager* AnimationManager::GetInstance(){
    static AnimationManager instance;
    return &instance;
}

Vector3 AnimationManager::CalculationValue(const std::vector<KeyFrameVector3>& keyFrames, float time) {
   
    assert(!keyFrames.empty());
   
    if (keyFrames.size() == 1 || time <= keyFrames[0].time) {
        return keyFrames[0].value;
    }

    for (size_t index = 0; index < keyFrames.size() - 1; ++index) {
        size_t nextIndex = index + 1;
        //indexとnextIndexの2つのkeyFrameを取得して範囲内に時刻があるかを判定
        if (keyFrames[index].time <= time && time <= keyFrames[nextIndex].time) {
            //範囲内を補間する
            float t = (time - keyFrames[index].time) / (keyFrames[nextIndex].time - keyFrames[index].time);
            //Vector3 だと線形補間
            return Lerp(keyFrames[index].value, keyFrames[nextIndex].value, t);
        }
    }

   
    return (*keyFrames.rbegin()).value;

}

Quaternion AnimationManager::CalculationValue(const std::vector<KeyFrameQuaternion>& keyFrames, float time) {
   
    assert(!keyFrames.empty());
   
    if (keyFrames.size() == 1 || time <= keyFrames[0].time) {
        return keyFrames[0].value;
    }

    for (size_t index = 0; index < keyFrames.size() - 1; ++index) {
        size_t nextIndex = index + 1;
        //indexとnextIndexの2つのkeyFrameを取得して範囲内に時刻があるかを判定
        if (keyFrames[index].time <= time && time <= keyFrames[nextIndex].time) {
            //範囲内を補間する
            float t = (time - keyFrames[index].time) / (keyFrames[nextIndex].time - keyFrames[index].time);
            //QuaternionだとSlerp
            return QuaternionSlerp(keyFrames[index].value, keyFrames[nextIndex].value, t);
        }
    }
   
    return (*keyFrames.rbegin()).value;
}



Animation AnimationManager::LoadAnimationFile(const std::string& directoryPath, const std::string& fileName) {
    Animation animation = {};
    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + fileName;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);

    //アニメーションが無い場合止める
    assert(scene->mNumAnimations != 0);
    //最初のアニメーションだけを採用
    aiAnimation* animationAssimp = scene->mAnimations[0];

    //mTicksPerSecond...周波数
    //mDuration...mTicksPerSecondで指定された周波数における長さ
    animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);



    //NodeAnimationを解析する

    for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
        aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
        NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
        //Translate
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
            KeyFrameVector3 keyFrame = {};
            //秒変換
            keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
            //右手->左手
            keyFrame.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
            nodeAnimation.translate.keyFrames.push_back(keyFrame);
        }

        //RotateはmNunRotateionKeys/mRotateKeys
        //Rotate
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
            aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
            KeyFrameQuaternion keyFrame = {};
            //秒変換
            keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
            //右手->左手
            //KeyFrame.value={rotate.x,-rotate.y,-rotate.z,rotate.w};
            keyFrame.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };
            nodeAnimation.rotate.keyFrames.push_back(keyFrame);
        }

        //ScaleはmNumScalingKeys/mScalingKeysで取得出来るので同時に行う

        //Scale
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
            aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
            KeyFrameVector3 keyFrame = {};
            //秒変換
            keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
            keyFrame.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
            nodeAnimation.scale.keyFrames.push_back(keyFrame);
        }

    }


    return animation;
}

uint32_t AnimationManager::LoadFile(const std::string& directoryPath, const std::string& fileName){

    for (uint32_t i = 0; i < ANIMATION_MAX_AMOUNT_; ++i) {
        if (AnimationManager::GetInstance()->animationInfromtion_[i].directoryPath == directoryPath &&
            AnimationManager::GetInstance()->animationInfromtion_[i].fileName == fileName) {
            return AnimationManager::GetInstance()->animationInfromtion_[i].handle;
        }
    }
    


    Animation animation = AnimationManager::GetInstance()->LoadAnimationFile(directoryPath, fileName);



    handle_++;

    AnimationManager::GetInstance()->animationInfromtion_[handle_].animationData = animation;
    AnimationManager::GetInstance()->animationInfromtion_[handle_].directoryPath = directoryPath;
    AnimationManager::GetInstance()->animationInfromtion_[handle_].fileName = fileName;
    AnimationManager::GetInstance()->animationInfromtion_[handle_].handle = handle_;


    return handle_;


}

void AnimationManager::ApplyAnimation(Skeleton& skeleton, uint32_t animationHandle, uint32_t modelHandle, float animationTime){


    Animation animationData = AnimationManager::GetInstance()->animationInfromtion_[animationHandle].animationData;
    ModelData modelData = ModelManager::GetInstance()->GetModelData(modelHandle);
    for (Joint& joint : skeleton.joints_) {
       
        if (auto it = animationData.nodeAnimations.find(joint.name); it != animationData.nodeAnimations.end()) {
            animationTime = std::fmodf(animationTime, animationData.duration);
            const NodeAnimation& rootNodeAnimation = (*it).second;
            joint.transform.translate = CalculationValue(rootNodeAnimation.translate.keyFrames, animationTime);
            joint.transform.rotate = CalculationValue(rootNodeAnimation.rotate.keyFrames, animationTime);
            joint.transform.scale = CalculationValue(rootNodeAnimation.scale.keyFrames, animationTime);
        }

    }
}


