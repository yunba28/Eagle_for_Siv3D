#pragma once

#include "Project.hpp"

namespace Project
{
	class DefaultScene : public GameWorld::Scene
	{
	public:

		/// @brief シーンのコンストラクタ
		DefaultScene(const InitData& _init);

		/// @brief シーンのデストラクタ
		virtual ~DefaultScene();

		/// @brief 通常時に呼び出される更新処理です
		virtual void update()override;

		/// @brief 通常時に呼び出される描画処理です
		virtual void draw()const override;
	
	};
}
