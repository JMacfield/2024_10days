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
