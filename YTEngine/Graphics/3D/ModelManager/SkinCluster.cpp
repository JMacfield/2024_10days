#include "SkinCluster.h"
#include <SrvManager.h>
#include <Matrix4x4Calculation.h>
#include <algorithm>
#include <PipelineManager.h>
#include "ModelManager.h"
#include <cassert>

void  SkinCluster::Create(const Skeleton& skeleton, const ModelData& modelData){
    skeleton_ = skeleton;
   
    paletteResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(WellForGPU) * skeleton_.joints_.size());
    WellForGPU* mappedPalette = nullptr;
    paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
   
    mappedPalette_ = { mappedPalette,skeleton_.joints_.size() };
    srvIndex_ = SrvManager::GetInstance()->Allocate();
    paletteSrvHandle_.first = SrvManager::GetInstance()->GetCPUDescriptorHandle(srvIndex_);
    paletteSrvHandle_.second = SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex_);


    //palette用のSRVを作成
    D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
    paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    paletteSrvDesc.Buffer.FirstElement = 0;
    paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    paletteSrvDesc.Buffer.NumElements = UINT(skeleton_.joints_.size());
    paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);

    //SRVを作る
    DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(
        paletteResource_.Get(), &paletteSrvDesc, paletteSrvHandle_.first);




    //influence用のResourceを確保
    //頂点ごとにinfluence情報
    influenceResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
    VertexInfluence* mappedInfluence = nullptr;
    influenceResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
   
    std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
    mappedInfluence_ = { mappedInfluence,modelData.vertices.size() };

    //Influence用のVertexBufferViewを作成
    influenceBufferView_.BufferLocation = influenceResource_->GetGPUVirtualAddress();
    influenceBufferView_.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
    influenceBufferView_.StrideInBytes = sizeof(VertexInfluence);

    influenceResource_->Unmap(0, nullptr);
    //InfluenceBindPoseMatrixの保存領域を作成
    inverseBindPoseMatrices.resize(skeleton_.joints_.size());
   
    std::generate(inverseBindPoseMatrices.begin(), inverseBindPoseMatrices.end(), MakeIdentity4x4);

#pragma region std::generateについて
   
#pragma endregion

    
    for (const auto& jointWeight : modelData.skinClusterData) {
      
        auto it = skeleton_.jointMap_.find(jointWeight.first);
        if (it == skeleton_.jointMap_.end()) {
           
            continue;
        }
     
        inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
        for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
           
            auto& currentInfluence = mappedInfluence_[vertexWeight.vertexIndex];
          
            for (uint32_t index = 0; index < NUM_MAX_INFLUENCE; ++index) {
               
                if (currentInfluence.weights[index] == 0.0f) {
                    currentInfluence.weights[index] = vertexWeight.weight;
                    currentInfluence.jointIndices[index] = (*it).second;
                    break;
                }

            }
        }


    }



}

void SkinCluster::Update(const Skeleton& skeleton){
    for (size_t jointIndex = 0; jointIndex < skeleton.joints_.size(); ++jointIndex) {
        assert(jointIndex < inverseBindPoseMatrices.size());
        //inverseBindPoseMatricesに原因あり！！
        mappedPalette_[jointIndex].skeletonSpaceMatrix =
            Multiply(inverseBindPoseMatrices[jointIndex], skeleton.joints_[jointIndex].skeletonSpaceMatrix);
        mappedPalette_[jointIndex].skeletonSpaceIncerseTransposeMatrix = 
            MakeTransposeMatrix(Inverse(mappedPalette_[jointIndex].skeletonSpaceMatrix));
    }


}
