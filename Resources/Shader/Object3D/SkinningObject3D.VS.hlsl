
struct Well{
    float32_t4x4 skeletonSpaceMatrix;
    float32_t4x4 skeletonSpaceInverseTransposeMatrix;
    
};

StructuredBuffer<Well> gMatrixPalette : register(t0);



//座標返還を行うVS
struct TransformationMatrix
{
	//32bitのfloatが4x4個
    float32_t4x4 world;
    float32_t4x4 normal;
    float32_t4x4 worldInverseTranspose;
};

struct Camera
{
	//必要なのはこの3つ
	//ビュー行列
    float32_t4x4 viewMatrix_;
	//射影行列
    float32_t4x4 projectionMatrix_;
	//正射影行列
    float32_t4x4 orthographicMatrix_;
};


//CBuffer
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);

struct VertexShaderInput{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t4 weight : WEIGHT0;
    int32_t4 index : INDEX0;
};


struct Skinned{
    float32_t4 position;
    float32_t3 normal;
};


struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
};

Skinned Skinning(VertexShaderInput input){
    Skinned skinned;
    
    //位置の変換
    skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f;
    
    //法線の変換
    skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal);
    
    
    
    
    return skinned;
}


VertexShaderOutput main(VertexShaderInput input){
    VertexShaderOutput output;
    //Skinninの計算を行って、Skinning語の頂点情報を手に入れる
    //ここでの頂点もSkeletonSpace
    Skinned skinned = Skinning(input);
    
    
    float32_t4x4 world = gTransformationMatrix.world;
    float32_t4x4 viewProjection = mul(gCamera.viewMatrix_, gCamera.projectionMatrix_);
	
    float32_t4x4 wvp = mul(world, viewProjection);
	
	//mul...組み込み関数
    output.position = mul(skinned.position, wvp);
    output.texcoord = input.texcoord;
	//法線の変換にはWorldMatrixの平衡移動は不要。拡縮回転情報が必要
	//左上3x3だけを取り出す
	//法線と言えば正規化をなのでそれを忘れないようにする
    output.normal = normalize(mul(skinned.normal, (float32_t3x3) gTransformationMatrix.worldInverseTranspose));
	
	//CameraWorldPosition
    output.worldPosition = mul(skinned.position, gTransformationMatrix.world).xyz;
    return output;
}