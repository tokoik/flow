#version 430 core

#define INVOCATIONS 32

layout(points, invocations = INVOCATIONS) in;
layout(triangle_strip, max_vertices = 4) out;

// 物理量のグリッド
unform sampler3D force;

// 粒子の影響半径
uniform float forceRadius;

void main(void)
{
  // 物理量のグリッドのサイズ
  ivec3 forceSize = textureSize(force, 0);

  // スライスの数
  int forceSlices = forceSize.z;

  // スライスの間隔
  float forceInterval = 2.0 / float(forceSlices);

  // スライスの前端の位置
  float forceFront = gl_in[0].gl_Position.z - forceRadius;

	// メタボールの中心とスライスとの距離のメタボールの半径に対する割合を求める
	d = (zSlice - p.z) / radius;

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
