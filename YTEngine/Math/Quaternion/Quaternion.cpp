#include "Quaternion.h"

#include <cmath>
#include <numbers>
#include "Vector3.h"
#include "VectorCalculation.h"


Quaternion QuaternionMultiply(const Quaternion& lhs, const Quaternion& rhs) {
    Quaternion result = {};
    Quaternion q = lhs;
    Quaternion r = rhs;

    //wはそのまま
    //xはi部分
    //yはj部分
    //zはk部分

    result.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
    result.x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    result.y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    result.z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;



    return result;
}

//単位
Quaternion IdentityQuaternion() {

    //e=1+0i+0j+0k
    //qe=eq=q

    Quaternion result = {};

    result.w = 1.0f;
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;





    return result;
}

//共役
Quaternion Conjugate(const Quaternion& quaternion) {

    //q*=w-xi-yj-zk=(qw,-qv)
    
    Quaternion result = {};
    result.x = -quaternion.x;
    result.y = -quaternion.y;
    result.z = -quaternion.z;
    result.w = quaternion.w;


    return result;
}

float Norm(const Quaternion& quaternion) {

    //||q||=sqrtf(qq*)
    float result = 0.0f;
    result = sqrtf(
        quaternion.w * quaternion.w +
        quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z);

    return result;
}

Quaternion Normalize(const Quaternion& quaternion) {
    Quaternion result = {};


    float length = sqrtf(
        quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z +
        quaternion.w * quaternion.w
    );


    Quaternion preResult = {};
    preResult.x = quaternion.x;
    preResult.y = quaternion.y;
    preResult.z = quaternion.z;
    preResult.w = quaternion.w;

    if (length != 0.0f) {
        preResult.x = quaternion.x / length;
        preResult.y = quaternion.y / length;
        preResult.z = quaternion.z / length;
        preResult.w = quaternion.w / length;

    }

    result.x = preResult.x;
    result.y = preResult.y;
    result.z = preResult.z;
    result.w = preResult.w;


    return result;
}

Quaternion Inverse(const Quaternion& quaternion) {
    //q^-1=q*
    //   ||q||^2


    Quaternion result = {};


    float norm = Norm(quaternion);
    Quaternion conjugate = Conjugate(quaternion);

    float t = norm * norm;

    result.x = conjugate.x / t;
    result.y = conjugate.y / t;
    result.z = conjugate.z / t;
    result.w = conjugate.w / t;

    return result;
}



Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

    //q=(cosθ/2,n*sinθ/2)
    Vector3 n = Normalize(axis);

    Quaternion result = {};
    result.w = std::cosf(angle / 2.0f);
    result.x = n.x * std::sinf(angle / 2.0f);
    result.y = n.y * std::sinf(angle / 2.0f);
    result.z = n.z * std::sinf(angle / 2.0f);


    return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
    //一番下の行はいらないね

    Quaternion convertQ = {};
    convertQ.w = 0.0f;
    convertQ.x = vector.x;
    convertQ.y = vector.y;
    convertQ.z = vector.z;


    Quaternion conjugate = Conjugate(quaternion);

    Quaternion preResult = {};
    preResult = QuaternionMultiply(quaternion, QuaternionMultiply(convertQ, conjugate));

    Vector3 result = { preResult.x,preResult.y,preResult.z };
    return result;
}

//Quaternion版の回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
    Matrix4x4 result = {};
    float w = quaternion.w;
    float x = quaternion.x;
    float y = quaternion.y;
    float z = quaternion.z;

    result.m[0][0] = (w * w) + (x * x) - (y * y) - (z * z);
    result.m[0][1] = 2.0f * (x * y + w * z);
    result.m[0][2] = 2.0f * (x * z - w * y);
    result.m[0][3] = 0.0f;

    result.m[1][0] = 2.0f * (x * y - w * z);
    result.m[1][1] = (w * w) - (x * x) + (y * y) - (z * z);
    result.m[1][2] = 2.0f * (y * z + w * x);
    result.m[1][3] = 0.0f;

    result.m[2][0] = 2.0f * (x * z + w * y);
    result.m[2][1] = 2.0f * (y * z - w * x);
    result.m[2][2] = (w * w) - (x * x) - (y * y) + (z * z);
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;


    return result;
}

//回転の補間
Quaternion QuaternionSlerp(Quaternion q0, Quaternion q1, float t) {

    //q0・q1=||q0||・||q1||*cosθ
    
    //q0・q1 = cosθ
    float dot =
        q0.x * q1.x +
        q0.y * q1.y +
        q0.z * q1.z +
        q0.w * q1.w;


  
    if (dot < 0.0f) {
       
        q0.x = -q0.x;
        q0.y = -q0.y;
        q0.z = -q0.z;
        q0.w = -q0.w;

        //内積も反転
        dot = -dot;
    }

    //dotが限りなく1に近い場合
    const float EPSILON = 0.0005f;
    if (dot > 1.0f - EPSILON) {
        // 直線補間を行う
        Quaternion result = {};
        result.x = (1.0f - t) * q0.x + t * q1.x;
        result.y = (1.0f - t) * q0.y + t * q1.y;
        result.z = (1.0f - t) * q0.z + t * q1.z;
        result.w = (1.0f - t) * q0.w + t * q1.w;
        return result;
    }

   
    //角度を求める
    float theta = std::acosf(dot);

    //Quaternionの前にある係数
   
    float scale0 = float(std::sin((1 - t) * theta) / std::sin(theta));
    float scale1 = float(std::sin(t * theta) / std::sin(theta));

    Quaternion result = {};
    result.x = scale0 * q0.x + scale1 * q1.x;
    result.y = scale0 * q0.y + scale1 * q1.y;
    result.z = scale0 * q0.z + scale1 * q1.z;
    result.w = scale0 * q0.w + scale1 * q1.w;

    return result;
}