//
// 粒子群オブジェクト
//
#include "Blob.h"

// コンストラクタ
Blob::Blob(const Particles &particles)
  : count(static_cast<GLsizei>(particles.size()))
  , drawShader(ggLoadShader("point.vert", "point.frag"))
  , mpLoc(glGetUniformLocation(drawShader, "mp"))
  , mvLoc(glGetUniformLocation(drawShader, "mv"))
{
  // 頂点配列オブジェクトを作成する
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 頂点バッファオブジェクトを作成する
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(Particle), nullptr, GL_STATIC_DRAW);

  // 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
  glVertexAttribPointer(0, std::tuple_size<Vector>::value, GL_FLOAT, GL_FALSE,
    sizeof(Particle), &static_cast<const Particle *>(0)->position);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, std::tuple_size<Vector>::value, GL_FLOAT, GL_FALSE,
    sizeof(Particle), &static_cast<const Particle *>(0)->velocity);
  glEnableVertexAttribArray(1);

  // 頂点バッファオブジェクトにデータを格納する
  initialize(particles);
}

// デストラクタ
Blob::~Blob()
{
  // 頂点配列オブジェクトを削除する
  glDeleteBuffers(1, &vao);

  // 頂点バッファオブジェクトを削除する
  glDeleteBuffers(1, &vbo);
}

// 初期化
void Blob::initialize(const Particles &particles) const
{
  // 頂点バッファオブジェクトを結合する
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  //  頂点バッファオブジェクトにデータを格納する
  Particle *p(static_cast<Particle *>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)));
  for (auto particle : particles)
  {
    p->position = particle.position;
    p->velocity = particle.velocity;
    ++p;
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
}

// 描画
void Blob::draw(const GgMatrix &mp, const GgMatrix &mv) const
{
  // 描画する頂点配列オブジェクトを指定する
  glBindVertexArray(vao);

  // 点のシェーダプログラムの使用開始
  glUseProgram(drawShader);

  // uniform 変数を設定する
  glUniformMatrix4fv(mpLoc, 1, GL_FALSE, mp.get());
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv.get());

  // 点で描画する
  glDrawArrays(GL_POINTS, 0, count);
}
