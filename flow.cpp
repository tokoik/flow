//
// アプリケーション本体
//
#include "GgApplication.h"

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("Flow");

  //
  // 描画の設定
  //

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

    // カラーバッファを入れ替える
    window.swapBuffers();
  }
}
