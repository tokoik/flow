//
// 粒子群オブジェクト
//
#include "Blob.h"

// 補間格子のサイズ
constexpr GLsizei GRID_X(64), GRID_Y(64), GRID_Z(64);

// 計算空間
constexpr GLfloat RANGE[] = { -3.0f, 3.0f, -3.0f, 3.0f, -3.0f, 3.0f };

// 粒子のサイズ
constexpr GgVector RADIUS = { 0.3f, 0.3f, 0.3f, 0.0f };

// コンストラクタ
Blob::Blob(const Particles &particles)
  : count(static_cast<GLsizei>(particles.size()))
  , drawShader(ggLoadShader("point.vert", "point.frag"))
  , mpLoc(glGetUniformLocation(drawShader, "mp"))
  , mvLoc(glGetUniformLocation(drawShader, "mv"))
  , mtLoc(glGetUniformLocation(drawShader, "mt"))
  , forceShader(ggLoadShader("force.vert", "force.frag", "force.geom"))
  , mtLoc(glGetUniformLocation(drawShader, "mt"))
  , gridLoc(glGetUniformLocation(drawShader, "grid"))
  , radiusLoc(glGetUniformLocation(drawShader, "grid"))
  , updateShader(ggLoadComputeShader("update.comp"))
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

  // フレームバッファオブジェクトとターゲットのテクスチャ
  glGenTextures(1, &texture);
  glGenFramebuffers(1, &target);

  // 三次元テクスチャを作る
  glBindTexture(GL_TEXTURE_3D, texture);
  glTexStorage3D(GL_TEXTURE_3D, 4, GL_RGBA32F, GRID_X, GRID_Y, GRID_Z);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

  // テクスチャの境界色
  static constexpr GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border);

  // 物理量のレンダリング先のフレームバッファオブジェクト
  glBindFramebuffer(GL_FRAMEBUFFER, target);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
}

// デストラクタ
Blob::~Blob()
{
  // 頂点配列オブジェクトを削除する
  glDeleteBuffers(1, &vao);

  // 頂点バッファオブジェクトを削除する
  glDeleteBuffers(1, &vbo);

  // フレームバッファオブジェクトを削除する
  glDeleteFramebuffers(1, &target);

  // 三次元テクスチャを削除する
  glDeleteTextures(1, &texture);
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
void Blob::draw(const GgMatrix &mp, const GgMatrix &mv, const GgMatrix &mt) const
{
  // 描画する頂点配列オブジェクトを指定する
  glBindVertexArray(vao);

  // 物理量を描画するフレームバッファに切り替える
  glBindFramebuffer(GL_FRAMEBUFFER, target);

  // 力の計算用のシェーダプログラムの使用開始
  glUseProgram(forceShader);

  // クリッピング空間中の粒子の大きさ
  const GgVector radius(mt * RADIUS);

  // uniform 変数を設定する
  glUniformMatrix4fv(mtLoc, 1, GL_FALSE, mt.get());
  glUniform1i(gridLoc, 0);
  glUniform3fv(radiusLoc, 1, radius.data());

  // 格子のテクスチャ
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, texture);

  // 点で描画する
  glDrawArrays(GL_POINTS, 0, count);

  // 通常のフレームバッファに戻す
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // 点のシェーダプログラムの使用開始
  glUseProgram(drawShader);

  // uniform 変数を設定する
  glUniformMatrix4fv(mpLoc, 1, GL_FALSE, mp.get());
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv.get());

  // 点で描画する
  glDrawArrays(GL_POINTS, 0, count);
}

// 更新
void Blob::update() const
{
  // シェーダストレージバッファオブジェクトを 0 番の結合ポイントに結合する
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);

  // 更新用のシェーダプログラムの使用開始
  glUseProgram(updateShader);

  // 計算を実行する
  glDispatchCompute(count, 1, 1);
}
