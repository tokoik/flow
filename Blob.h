#pragma once
#include "Particle.h"

// 標準ライブラリ
#include <vector>

//
// 粒子群データ
//
using Particles = std::vector<Particle>;

//
// 粒子群オブジェクト
//
class Blob
{
  // 頂点配列オブジェクト名
  GLuint vao;

  // 頂点バッファオブジェクト名
  GLuint vbo;

  // 頂点の数
  const GLsizei count;

  // 描画用のシェーダ
  const GLuint drawShader;

  // unform 変数の場所
  const GLint mpLoc, mvLoc, mtLoc;

  // 影響力計算用のシェーダ
  const GLuint forceShader;

  // uniform 変数の場所
  const GLint mtLoc, gridLoc, radiusLoc;

  // 格子の三次元テクスチャ
  GLuint texture;

  // 物理量のレンダリング先
  GLuint target;

  // 更新用のシェーダ
  const GLuint updateShader;

public:

  // コンストラクタ
  Blob(const Particles &particles);

  // デストラクタ
  virtual ~Blob();

  // コピーコンストラクタによるコピー禁止
  Blob(const Blob &blob) = delete;

  // 代入によるコピー禁止
  Blob &operator=(const Blob &blob) = delete;

  // 初期化
  void initialize(const Particles &particles) const;

  // 描画
  void draw(const GgMatrix &mp, const GgMatrix &mv, const GgMatrix &mt) const;

  // 更新
  void update() const;
};
