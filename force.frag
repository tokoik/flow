#version 430 core

// フレームバッファに出力するデータ
layout (location = 0) out vec4 color;               // フラグメントの色

// 粒子の大きさをクリッピング空間中からワールド座標に変換するスケール
uniform vec3 scale;

// この invocation のレイヤの粒子からの変位
in float offset;

void main()
{
	// このフラグメントのスライス上の位置
	vec3 q = vec3(gl_PointCoord * vec2(2.0, -2.0) - vec2(1.0, -1.0), offset);

	// そのメタボールの中心からの距離
	float r = length(q);

	// メタボールとスライスが交差していなければフラグメントを捨てる
	if (r > 1.0) discard;

	// 勾配と濃度
	fc = vec4((1.0 - r) * 6.0 * q, smoothstep(1.0, 0.0, r));

  // フラグメントの色の出力
  color = vec4(1.0);
}
