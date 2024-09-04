#pragma once
#include <Vector3.h>
#include "Matrix4x4.h"

//Vector3

//足し算
Vector3 Add(Vector3 v1, Vector3 v2);

//引き算
Vector3 Subtract(Vector3 v1, Vector3 v2);

/// <summary>
/// クランプ
/// </summary>
/// <param name="t"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
float Clamp(float t, float min, float max);

/// <summary>
/// 内積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float DotVector3(const Vector3 v1, const Vector3 v2);
float DotVector2(const Vector3 v1, const Vector3 v2);

float Length(Vector3 V1);
Vector3 Normalize(Vector3 V1);

/// <summary>
/// 線形補間(float)
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
float Lerp(float start, float end, float t);

/// <summary>
/// 線形補間(Vector3)
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(Vector3 start, Vector3 end, float t);

/// <summary>
/// 球面線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);


float Cot(float theta);

//外積
Vector3 Cross(const Vector3 v1, const Vector3 v2);


Vector3 Project(const Vector3 v1, const Vector3 v2);

Vector3 TransformCalculation(const Vector3 vector, const Matrix4x4 matrix);