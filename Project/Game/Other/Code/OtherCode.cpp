#include "OtherCode.h"

int32_t OtherCode::GetDigits(int32_t num) {

    // 桁数
    int32_t result = 0;
    // 除算後の数値を保持する変数
    int32_t value = num;

    while (value > 0)
    {
        // 10で割る
        value /= 10;
        // 桁数を加算
        result++;
    }
    return result;
}

float OtherCode::ExponentialInterpolation(float a, float b, float t, float k) {
    float result = 0.0f;
    result = a + ((b - a) * k * t);

    return result;
}

Vector3 OtherCode::ExponentialInterpolation(Vector3 a, Vector3 b, float t, float k) {
    Vector3 result = {};
    result.x = a.x + ((b.x - a.x) * k * t);
    result.y = a.y + ((b.y - a.y) * k * t);
    result.z = a.z + ((b.z - a.z) * k * t);
    return result;
}
