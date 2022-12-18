#pragma once

#include <Core/Component/Component.hpp>
#include <Utility/Name/Name.hpp>
#include <Utility/Tag/Tag.hpp>

namespace eagle
{
	class Actor : Uncopyable
	{
	private:

		using Scene_ref = WeakObject<Internal::ScenePimpl>;

		using Scene_handle = ObjectHandle<Internal::ScenePimpl>;

		using Actor_ref = WeakObject<Actor>;

		using Actor_handle = ObjectHandle<Actor>;

		using Component_ref = WeakObject<Component>;

		using Component_handle = ObjectHandle<Component>;

		using Transform_ref = WeakObject<Transform>;

		using Transform_handle = ObjectHandle<Transform>;

		using TagList = Array<Tag>;

	public:

		Actor();

		virtual ~Actor();

		/// @brief Actorに親を設定します 
		/// @brief 空の参照を渡すと親子関係を外します 
		void setParent(const Actor_ref& _parent);

		/// @brief Actorの名前を設定する
		void setName(const String& _name);

		/// @brief Actorにタグを追加する
		void addTag(const String& _tag);

		/// @brief Actorから文字列と等しいタグを削除する
		void removeTag(const String& _tag);

		/// @brief Actorの有効・無効を設定する
		void setActive(bool _active)noexcept;

		/// @brief Actorが所属しているシーンの参照を取得する
		Scene_handle getScene()const noexcept;

		/// @brief Actorが所有するTransformの参照を取得する
		Transform_handle getTransform()const noexcept;

		/// @brief Actorの親を取得
		/// @return 見つからなければ空のActorを取得します
		Actor_handle getParent()const noexcept;

		/// @brief Actorの一番上の親を取得
		/// @return 見つからなければ空のActorを取得します
		Actor_handle getRoot()const noexcept;

		/// @brief Actorの名前を取得すつ
		const String& getName()const;

		/// @brief Actorの持つタグを取得する
		/// @param _index リスト内でのインデックス
		const String& getTag(size_t _index)const;

		/// @brief Actorが持つタグをすべて列挙する
		Array<String> enumrateTagList()const;

		/// @brief 文字列をActorの名前と比較する
		/// @param _name 比較する文字列
		bool compareName(const String& _name)const noexcept;

		/// @brief Actorが持つタグのいずれかと文字列が等しいか比較する
		/// @param _tag 比較するタグ
		bool compareTag(const String& _tag)const noexcept;

		/// @brief Actorが有効かどうか
		bool isActive()const noexcept;

		/// @brief Actorに破棄命令がでているか
		bool isPendingKill()const noexcept;

		/// @brief ActorにComponentをアタッチする
		/// @tparam T Componentの型
		/// @return 作成したComponentの弱参照
		template<class T>
		WeakObject<T> attachComponent()requires(std::derived_from<T, Component>);

		/// @brief ActorにアタッチされているComponentを取得する
		/// @tparam T Componentの型
		/// @return 見つからなければ空のWeakObjectが返る
		template<class T>
		WeakObject<T> getComponent()requires(std::derived_from<T, Component>);

		/// @brief ActorにアタッチされているComponentをディタッチする
		/// @tparam T Componentの型
		template<class T>
		void detachComponent()requires(std::derived_from<T, Component>);

	public:

		virtual void awake() {}

		virtual void dispose() {}

		virtual void _internalAwake();

		virtual void _internalDispose();

	public:

		template<class T = Actor>
		static SharedObject<T> Create(const Scene_ref& scene, const String& name)requires(std::derived_from<T, Actor>);

		static void Destroy(const Actor_ref& actor);

		static void Destroy(const Actor_handle& actor);

	private:

		static void Create_impl(
			const Scene_ref& scene,
			const SharedObject<Actor>& actor,
			const std::type_index& type,
			const String& name);

	protected:

		/// @brief 自身の参照
		Actor_ref mThis;

	private:

		/// @brief 所属しているシーンの参照
		Scene_ref mScene;

		/// @brief 所有しているTransformの参照
		Transform_ref mTransform;

		/// @brief Actorの名前
		Name mName;

		/// @brief Actorが持つタグ
		TagList mTagList;

		/// @brief ActorにアタッチされたComponentの参照
		Array<Component_ref> mComponents;

		/// @brief Actorの有効・無効
		bool mIsActive;

		/// @brief Actorが破棄待ちかどうか
		bool mIsPendingKill;
	};
}

#include "Actor.ipp"
