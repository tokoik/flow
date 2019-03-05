#version 430 core

#define INVOCATIONS 32

layout(points, invocations = INVOCATIONS) in;
layout(triangle_strip, max_vertices = 4) out;

// 物理量のグリッド
unform sampler3D grid;

// 領域の前端 (x: zmin) と奥行き (y : zmax - zmin)
uniform vec2 range;

// 粒子の影響半径
uniform vec3 radius;

void main(void)
{
  // 物理量のグリッドのサイズ
  ivec3 size = textureSize(grid, 0);

  // スライスの数 - 1
  int slices = size.z - 1;

  // この粒子の基準のスライス
  int origin = int((gl_in[0].gl_Position.z - range.x) * slices / range.y + 0.5);

  // この invocation のスライス
  gl_layer = origin + gl_InvocationID - INVOCATIONS / 2;

  // 計算領域より手前なら戻る
  if (gl_layer < 0) return;

  // 計算領域より後ろなら戻る
  if (gl_layer > slices) return;

  // この invocation のスライスの位置
  float depth = range.x + range.y * gl_Layer / slices;



  // スライスの間隔
  const interval = 2.0 / float(slices);

  // スライスの前端の位置
  float front = gl_in[0].gl_Position.z - radius.z;

  // スライスの後端の位置
  float rear = gl_in[0].gl_Position.z + radius.z;

	// メタボールの中心とスライスとの距離のメタボールの半径に対する割合を求める
	d = (slice - p.z) / radius.z;

	// メタボールの断面の大きさ（メタボールが必ずスライスと交差するなら 1.0 - d * d は負にならない）
	gl_PointSize = -size.y * 2.0 * radius * sqrt(1.0 - d * d) * mp[1][1] / zSlice;

  // レンダリングするレイヤ
  gl_Layer = gl_InvocationID + shift * INVOCATIONS;

  // 変位量
  delta = gl_Layer * step / INVOCATIONS;

  for (int i = 0; i < gl_in.length(); ++i)
  {
    // 正距方位図上の位置
    gl_Position = gl_in[i].gl_Position;

    // 基準のテクスチャ座標
    tc0 = gl_Position.xy * scale + offset;

    // 緯度をずらす
    vec2 pq = st[i] + vec2(0.0, delta);

    // 極座標から直交座標に変換する
    vec2 sin_pq = sin(pq), cos_pq = cos(pq);
    vec2 xy = vec2(-sin_pq.t * cos_pq.s, cos_pq.t);
    float z = sin_pq.t * sin_pq.s;

    // xy 平面上における頂点の中心からの距離の二乗
    float r = dot(xy, xy);
    if (r > 0.0) xy /= sqrt(r);

    // 正距方位図上の位置
    vec2 uv = 2.0f * acos(-z) * xy / 3.1415927;

    // 対象のテクスチャ座標
    tc1 = uv * scale + offset;

    // 頂点データを生成する
    EmitVertex();
  }

  // 図形データ終了
  EndPrimitive();
}
