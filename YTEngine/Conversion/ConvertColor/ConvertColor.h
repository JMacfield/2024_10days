#pragma once

#include "Vector4.h"

const unsigned int WHITE =0xFFFFFFFF; // 白
const unsigned int BLACK =0x000000FF; // 黒
const unsigned int RED =0xFF0000FF;   // 赤
const unsigned int GREEN =0x00FF0FF;  // 緑
const unsigned int BLUE = 0x0000FFFF; // 青

const unsigned int  YELLOW = 0xFFFF00FF; //黄色
const unsigned int PURPLE = 0xFF00FFFF; //紫

Vector4 ColorAdapter(unsigned int color);