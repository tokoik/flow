//
// アプリケーション本体
//
#include "GgApplication.h"

// 粒子群オブジェクト
#include "Blob.h"

// 標準ライブラリ
#include <memory>
#include <random>

// 生成する粒子群の数
const int bCount(8);

// 生成する粒子群の中心位置の範囲
const GLfloat bRange(1.5f);

// 一つの粒子群の粒子数
const int pCount(1000);

// 一つの粒子群の中心からの距離の平均
const GLfloat pMean(0.0f);

// 一つの粒子群の中心からの距離の標準偏差
const GLfloat pDeviation(0.3f);

//
// 粒子群の生成
//
//   paticles 粒子群の格納先
//   count 粒子群の粒子数
//   cx, cy, cz 粒子群の中心位置
//   rn メルセンヌツイスタ法による乱数
//   mean 粒子の粒子群の中心からの距離の平均値
//   deviation 粒子の粒子群の中心からの距離の標準偏差
//
void generateParticles(Particles &particles, int count,
  GLfloat cx, GLfloat cy, GLfloat cz,
  std::mt19937 &rn, GLfloat mean, GLfloat deviation)
{
  // 一様実数分布
  //   [0, 2) の値の範囲で等確率に実数を生成する
  std::uniform_real_distribution<GLfloat> uniform(0.0f, 2.0f);

  // 正規分布
  //   平均 mean、標準偏差 deviation で分布させる
  std::normal_distribution<GLfloat> normal(mean, deviation);

  // 格納先のメモリをあらかじめ確保しておく
  particles.reserve(particles.size() + count);

  // 原点中心に直径方向に正規分布する粒子群を発生する
  for (int i = 0; i < count; ++i)
  {
    // 緯度方向
    const GLfloat cp(uniform(rn) - 1.0f);
    const GLfloat sp(sqrt(1.0f - cp * cp));

    // 経度方向
    const GLfloat t(3.1415927f * uniform(rn));
    const GLfloat ct(cos(t)), st(sin(t));

    // 粒子の粒子群の中心からの距離 (半径)
    const GLfloat r(normal(rn));

    // 粒子を追加する
    particles.emplace_back(r * sp * ct + cx, r * sp * st + cy, r * cp + cz);
  }
}

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("Flow");

  //
  // 粒子群オブジェクトの作成
  //

  // 乱数の種に使うハードウェア乱数
  //std::random_device seed;

  // メルセンヌツイスタ法による乱数の系列を設定する
  //std::mt19937 rn(seed());
  std::mt19937 rn(54321);

  // 粒子群データの初期値
  Particles initial;

  // 一様実数分布
  //   [-1.0, 1.0) の値の範囲で等確率に実数を生成する
  std::uniform_real_distribution<GLfloat> center(-bRange, bRange);

  // 発生する粒子群の数だけ繰り返す
  for (int i = 0; i < bCount; ++i)
  {
    // 点の玉中心位置
    const GLfloat cx(center(rn)), cy(center(rn)), cz(center(rn));

    // 中心からの距離に対して密度が正規分布に従う点の玉を生成する
    generateParticles(initial, pCount, cx, cy, cz, rn, pMean, pDeviation);
  }

  // 粒子群オブジェクトを作成する
  std::unique_ptr<const Blob> blob(new Blob(initial));

  //
  // 描画の設定
  //

  // ビュー変換行列を求める
  const GgMatrix view(ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

  // 背面カリングを有効にする
  glEnable(GL_CULL_FACE);

  // デプスバッファを有効にする
  glEnable(GL_DEPTH_TEST);

  // 背景色を指定する
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // 時計をリセットする
  glfwSetTime(0.0);

  //
  // 描画
  //
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 透視投影変換行列を求める
    const GgMatrix projection(ggPerspective(1.0f, window.getAspect(), 1.0f, 10.0f));

    // モデル変換行列を求める
    const GgMatrix model(window.getTrackball());

    // モデルビュー変換行列を求める
    const GgMatrix modelview(view * model);

    // 粒子群オブジェクトを描画する
    blob->draw(projection, modelview);

    // カラーバッファを入れ替える
    window.swapBuffers();
  }
}
