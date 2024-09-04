#include "Modelmanager.h"
#include <cassert>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ReadNode.h>

#include "Matrix4x4Calculation.h"

static uint32_t modelhandle;

ModelManager* ModelManager::GetInstance() {
	
	static ModelManager instance;
	return &instance;
}
//モデルデータの読み込み
ModelData ModelManager::LoadFile(const std::string& directoryPath, const std::string& fileName) {
	
	ModelData modelData;
	
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	//メッシュがないのは対応しない
	assert(scene->HasMeshes());

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		//Normalなので法線がない時は止める
		assert(mesh->HasNormals());
		//TextureCoordsなのでTexCoordが無い時は止める
		assert(mesh->HasTextureCoords(0));
		//頂点を解析する
		//最初に頂点数分のメモリを確保しておく
		modelData.vertices.resize(mesh->mNumVertices);
		for (uint32_t verticesIndex = 0; verticesIndex < mesh->mNumVertices; ++verticesIndex) {
			aiVector3D& position = mesh->mVertices[verticesIndex];
			aiVector3D& normal = mesh->mNormals[verticesIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][verticesIndex];
			//右手から左手への変換
			modelData.vertices[verticesIndex].position = { -position.x,position.y,position.z,1.0f };
			modelData.vertices[verticesIndex].normal = { -normal.x,normal.y,normal.z };
			modelData.vertices[verticesIndex].texCoord = { texcoord.x,texcoord.y };



		}
		//Indexの解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			//三角形で
			assert(face.mNumIndices == 3);
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData.indices.push_back(vertexIndex);
			}
		}

		//SkinCluster構築用のデータ取得を追加
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			//Jointごとの格納領域を作る
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

			//InverseBindPoseMatrixの抽出
			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale;
			aiVector3D translate;
			aiQuaternion rotate;

			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);

			Vector3 scaleAfter = { scale.x,scale.y,scale.z };
			Vector3 translateAfter = { -translate.x,translate.y,translate.z };
			Quaternion rotateQuaternion = { rotate.x,-rotate.y,-rotate.z,rotate.w };

			Matrix4x4 scaleMatrix = MakeScaleMatrix(scaleAfter);
			Matrix4x4 rotateMatrix = MakeRotateMatrix(rotateQuaternion);
			Matrix4x4 translateMatrix = MakeTranslateMatrix(translateAfter);


			
			Matrix4x4 bindPoseMatrix = Multiply(scaleMatrix, Multiply(rotateMatrix, translateMatrix));
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			//Weight情報を取り出す
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}


		}

	}



	//Materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}
	//ノードの読み込み
	modelData.rootNode = ReadNode::GetInstance()->Read(scene->mRootNode);
	//ModelDataを返す
	return modelData;
}

uint32_t ModelManager::LoadModelFile(const std::string& directoryPath, const std::string& fileName) {


	for (uint32_t i = 0; i < MODEL_MAX_AMOUNT_; i++) {
		//同じモデルを探す
		if (ModelManager::GetInstance()->modelInfromtion_[i].directoryPath == directoryPath &&
			ModelManager::GetInstance()->modelInfromtion_[i].filePath == fileName) {
			return ModelManager::GetInstance()->modelInfromtion_[i].handle;
		}
	}

	modelhandle++;

	//モデルの読み込み
	ModelData newModelData = ModelManager::GetInstance()->LoadFile(directoryPath, fileName);

	//新規登録
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].modelData = newModelData;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].animationData = {};
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].directoryPath = directoryPath;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].filePath = fileName;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].handle = modelhandle;

	//値を返す
	return modelhandle;
}

uint32_t ModelManager::LoadModelFile(const std::string& directoryPath, const std::string& fileName, bool isAnimationLoad) {
	
	for (uint32_t i = 0; i < MODEL_MAX_AMOUNT_; i++) {
		//同じモデルを探す
		if (ModelManager::GetInstance()->modelInfromtion_[i].directoryPath == directoryPath &&
			ModelManager::GetInstance()->modelInfromtion_[i].filePath == fileName) {
			return ModelManager::GetInstance()->modelInfromtion_[i].handle;
		}
	}
	
	modelhandle++;

	//モデルの読み込み
	ModelData newModelData = ModelManager::GetInstance()->LoadFile(directoryPath, fileName);

	Animation newAnimation = {};
	if (isAnimationLoad == true) {
		//アニメーションの読み込み
		newAnimation = LoadAnimationFile(directoryPath, fileName);

	}
	
	//新規登録
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].modelData = newModelData;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].animationData = newAnimation;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].directoryPath = directoryPath;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].filePath = fileName;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].handle = modelhandle;

	//値を返す
	return modelhandle;
}
