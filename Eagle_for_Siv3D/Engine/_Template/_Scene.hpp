#pragma once

#include <Framework/GameWorld.hpp>

class MyScene : public eagle::GameWorld<String, void>::Scene
{
public:

	/// @brief シーンのコンストラクタ
	MyScene(const InitData& _init);

	/// @brief シーンのデストラクタ
	virtual ~MyScene();

	/// @brief 通常時に呼び出される更新処理です
	virtual void update()override;

	/// @brief 通常時に呼び出される描画処理です
	virtual void draw()const override;

};
