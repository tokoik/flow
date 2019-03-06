#version 430 core

#define INVOCATIONS 32

layout(points, invocations = INVOCATIONS) in;
layout(points, max_vertices = 1) out;

// 物理量のグリッド
unform sampler3D grid;

// テクスチャ変換行列
uniform mat4 mt;

// クリッピング空間中の粒子の影響半径
uniform vec3 radius;

// 粒子が存在するレイヤ
in int layer[];

// この invocation のレイヤの粒子からの変位
out float offset;

void main(void)
{
  // この invocation のレイヤ
  gl_layer = layer[0] + gl_InvocationID - INVOCATIONS / 2;

  // 計算領域より手前なら戻る
  if (gl_layer < 0) return;

  // グリッドの大きさ
  ivec3 size = textureSize(grid, 0);

  // レイヤの数 - 1
  int slices = size.z - 1;

  // 計算領域より後ろなら戻る
  if (gl_layer > slices) return;

  // この invocation のレイヤの位置
  float z = gl_Layer * 2.0 / float(slices) - 1.0;

  // この invocation のレイヤの粒子の中心からの距離
  float d = abs(gl_in[0].gl_Position.z - z);

  // この invocation のレイヤが粒子の影響範囲外なら戻る
  float r = radius.z - d;
  if (r <= 0.0) return;

  // この invocation のレイヤの粒子からの変位
  float offset = -d / mt[2][2];

  // この invocation における点の大きさ (radius.z + d) * (radius.z - d)
  gl_PointSize = sqrt((radius.z + d) * r) * float(max(size.x, size.y)) * 2.0;

  // クリッピング空間中の粒子の位置
  gl_Position = gl_in[0].gl_Position;

  // 頂点を出力する
  EmitVertex();

  // 図形データ終了
  EndPrimitive();
}
