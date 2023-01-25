#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Framework/Detail/UniqueTag.hpp>
#include <Framework/Detail/CommonTag.hpp>
#include <Common/SmartObject.hpp>

#include <Siv3D/Camera2D.hpp>
#include <Siv3D/DebugCamera3D.hpp>
#include <Siv3D/Color.hpp>

using s3d::Camera2D;
using s3d::DebugCamera3D;
using s3d::Color;

namespace eagle::backend
{
	class SceneObjectDetail
	{
	public:

		SceneObjectDetail();

		virtual ~SceneObjectDetail();

		/// @brief T型のActorを作成する
		/// @tparam T Actorの派生クラス
		/// @param _name Actorの名前
		template<class T = Actor>
		WeakObject<T> createActor(const String& _name = U"MyActor")requires(std::derived_from<T, Actor>);

		/// @brief T型のComponentを作成する
		/// @tparam T Componentの派生クラス
		/// @param _actor Componentの所有者
		template<class T>
		WeakObject<T> createComponent(const WeakObject<Actor>& _actor)requires(std::derived_from<T, Component>);

		/// @brief 生成したActorをシーンに追加する
		/// @brief *ユーザーから使うことは通常ありません
		/// @param _actor 追加するActor
		/// @param _type 追加するActorの型情報
		void addActor(const SharedObject<Actor>& _actor, const TypeInfo& _type);

		/// @brief 生成したComponentをシーンに追加する
		/// @brief *ユーザーから使うことは通常ありません
		/// @param _component 追加するComponent
		/// @param _type 追加するComponentの型情報
		void addComponent(const SharedObject<Component>& _component, const TypeInfo& _type);

		/// @brief 生成したRenderer2DをRenderSysytemに追加する
		/// @brief *ユーザーから使うことは通常ありません
		/// @param _renderer 追加するRenderer2D
		void addRenderer(const WeakObject<Renderer2D>& _renderer);

		/// @brief 生成したRenderer3DをRenderSysytemに追加する
		/// @brief *ユーザーから使うことは通常ありません
		/// @param _renderer 追加するRenderer3D
		void addRenderer(const WeakObject<Renderer3D>& _renderer);

		/// @brief Renderer2Dの描画順が変更されたことを通知する
		void notifyChangeDrawOrder();

		/// @brief UniqueTagに固有の名前を発行する
		/// @brief 既存の名前を指定した場合は ' #n ' の形で連番になります
		/// @param _name 発行したい名前
		/// @param _tag 名前を設定するUniqueTag
		void securedName(const String& _name, UniqueTag& _tag);

		/// @brief CommonTagにタグを発行する
		/// @param _name 発光したいタグ
		/// @param _tag タグを設定するCommonTag
		void securedTag(const String& _name, CommonTag& _tag);

		/// @brief 画面の背景色を設定する
		void setBackgroundColor(const Color& _background);

		/// @brief 2Dカメラの弱参照を取得する
		WeakObject<Camera2D> getCamera2D()const noexcept;

		/// @brief 3Dカメラの弱参照を取得する
		WeakObject<DebugCamera3D> getCamera3D()const noexcept;

		/// @brief 画面の背景色を取得する
		const Color& getBackgroundColor()const noexcept;

	public:

		void update();

		void draw()const;

	private:

		WeakObject<SceneObjectDetail> mSelf;

		UniqueObject<class ActorSystem> mActorSystem;

		UniqueObject<class ComponentSystem> mComponentSystem;

		UniqueObject<class RenderSystem> mRenderSystem;

		UniqueTagAsset mNameAsset;

		CommonTagAsset mTagAsset;

		friend class SceneObject;
	};

	template<class T>
	WeakObject<T> SceneObjectDetail::createActor(const String& _name) requires(std::derived_from<T, Actor>)
	{
		auto result = MakeShared<T>();
		auto actor = Cast<Actor>(result);
		addActor(actor, typeid(T));
		Actor::_InternalSetup(mSelf, actor, _name);// down cast record
		return result.weak();
	}

	template<class T>
	WeakObject<T> SceneObjectDetail::createComponent(const WeakObject<Actor>& _actor) requires(std::derived_from<T, Component>)
	{

	}
}
