#version 430 core

// 頂点属性
layout (location = 0) in vec4 position;

// 物理量のグリッド
unform sampler3D grid;

// テクスチャ変換行列
uniform mat4 mt;

// 粒子が存在するレイヤ
out int layer;

void main()
{
  // 計算領域をクリッピング空間にはめ込む
  gl_Position = mt * position;

  // 粒子が存在するレイヤを求める
  layer = int((gl_Position.z + 1.0) * float(textureSize(grid, 0).z - 1) * 0.5 + 0.5);
}
