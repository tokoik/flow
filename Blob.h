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
  void draw() const;
};
