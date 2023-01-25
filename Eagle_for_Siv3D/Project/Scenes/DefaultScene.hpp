#pragma once

#include "Project.hpp"

namespace Project
{
	class DefaultScene : public EagleScene
	{
	public:

		/// @brief シーンのコンストラクタ
		DefaultScene(const InitData& _init);

		/// @brief シーンのデストラクタ
		virtual ~DefaultScene();

		/// @brief シーンが構築されたときに呼び出されます
		virtual void awake()override;

		/// @brief シーンが破棄されたときに呼びだされます
		virtual void dispose()override;

		/// @brief 通常時に呼び出される更新処理です
		virtual void update()override;

		/// @brief 通常時に呼び出される描画処理です
		virtual void draw()const override;
	
	};
}
