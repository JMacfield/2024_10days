#include "ConvertColor.h"

Vector4 ColorAdapter(unsigned int color)
{
    Vector4 ResultColor = 
    {
       ((color >> 24) & 0xFF) / 255.0f, // 赤

       ((color >> 16) & 0xFF) / 255.0f, // 緑

       ((color >>8) & 0xFF) / 255.0f,  // 青

       ((color) & 0xFF) / 255.0f //透明度
    };

     return ResultColor;
}