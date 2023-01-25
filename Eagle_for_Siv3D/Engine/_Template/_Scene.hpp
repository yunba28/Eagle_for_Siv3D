#pragma once

#include <Framework/BasicScene.hpp>

class MyScene : public eagle::BasicScene<String, void>
{
public:

	/// @brief シーンのコンストラクタ
	MyScene(const InitData& _init);

	/// @brief シーンのデストラクタ
	virtual ~MyScene();

	/// @brief シーンが構築されたときに呼び出されます
	virtual void awake()override;

	/// @brief シーンが破棄されたときに呼びだされます
	virtual void dispose()override;

	/// @brief 通常時に呼び出される更新処理です
	virtual void update()override;

	/// @brief 通常時に呼び出される描画処理です
	virtual void draw()const override;

};
