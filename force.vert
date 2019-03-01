#version 430 core

// 頂点属性
layout (location = 0) in vec4 position;             // 現在の位置

// テクスチャ変換行列
uniform mat4 mt;                                    // 計算領域

void main()
{
  // 計算領域をクリッピング空間にはめ込む
  gl_Position = mt * position;
}
