#pragma once

#include "Project.hpp"

class EasyConfigScene : public GameWorld::Scene
{
public:

	/// @brief シーンのコンストラクタ
	EasyConfigScene(const InitData& _init);

	/// @brief シーンのデストラクタ
	virtual ~EasyConfigScene();

	/// @brief 通常時に呼び出される更新処理です
	virtual void update()override;

	/// @brief 通常時に呼び出される描画処理です
	virtual void draw()const override;

};
