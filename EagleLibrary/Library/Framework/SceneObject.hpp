#pragma once

#include <Classes/FixedUpdate.hpp>
#include <Framework/Actor.hpp>

namespace eagle
{
	namespace Internal
	{
		class SceneObjectDetail
		{
		public:

			SceneObjectDetail();

			~SceneObjectDetail();

		private:

			void update();

		public:

			/// @brief T型のActorを作成する
			/// @tparam T Actorの派生クラス
			/// @param _name Actorの名前
			template<class T = Actor>
			WeakObject<T> createActor(const std::u32string& _name = U"Actor");

			/// @brief T型のComponentを作成する
			/// @tparam T Componentの派生クラス
			/// @param _actor Componentの所有者
			template<class T>
			WeakObject<T> createComponent(const WeakObject<Actor>& _actor);

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

			/// @brief シーンに所属するActorの数を返します
			size_t countActor()const noexcept;

			/// @brief シーンに所属するT型のActorの数を取得します
			/// @tparam T Actorの派生クラス
			template<class T, std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
			size_t countActor()const noexcept;

			/// @brief シーンに所属するComponentの数を取得します
			size_t countComponent()const noexcept;

			/// @brief シーンに所属するT型のComponentの数を取得します
			/// @tparam T Componentの派生クラス
			template<class T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
			size_t countComponent()const noexcept;

			/// @brief UniqueTagに固有の名前を発行する
			/// @brief 既存の名前を指定した場合は ' #n ' の形で連番になります
			/// @param _name 発行したい名前
			/// @param _tag 名前を設定するUniqueTag
			void securedName(const std::u32string& _name, UniqueTag& _tag);

			/// @brief CommonTagにタグを発行する
			/// @param _name 発光したいタグ
			/// @param _tag タグを設定するCommonTag
			void securedTag(const std::u32string& _name, CommonTag& _tag);

		private:

			static void _InternalSetup(const SharedObject<SceneObjectDetail>& scene);

		private:

			WeakObject<SceneObjectDetail> mSelf;

			UniqueObject<class ActorSystem> mActorSystem;

			UniqueObject<class ComponentSystem> mComponentSystem;

			UniqueTagAsset mNameAsset;

			CommonTagAsset mTagAsset;

			friend class SceneObject;

		};

		template<class T>
		WeakObject<T> SceneObjectDetail::createActor(const std::u32string& _name)
		{
			auto result = Actor::_InternalCreate<T>();
			auto actor = Cast<Actor>(result);
			addActor(actor, typeid(T));
			Actor::_InternalSetup(mSelf, actor, _name);
			return result.weak();
		}

		template<class T>
		WeakObject<T> SceneObjectDetail::createComponent(const WeakObject<Actor>& _actor)
		{
			auto result = Component::_InternalCreate<T>();
			auto component = Cast<Component>(result);
			addComponent(component, typeid(T));
			Component::_InternalSetup(_actor, component, typeid(T));
			return result.weak();
		}

		template<class T, std::enable_if_t<std::is_base_of_v<Actor, T>, int>>
		size_t SceneObjectDetail::countActor()const noexcept
		{
			return countActor(typeid(T));
		}

		template<class T, std::enable_if_t<std::is_base_of_v<Component, T>, int>>
		size_t SceneObjectDetail::countComponent()const noexcept
		{
			return countComponent(typeid(T));
		}
	}

	class SceneObject final : Uncopyable
	{
	public:

		SceneObject();

		~SceneObject();

		/// @brief シーンが管理するActorとComponentの更新を行う
		void update();

		/// @brief T型のActorを作成する
		/// @tparam T Actorの派生クラス
		/// @param _name Actorの名前
		template<class T = Actor>
		WeakObject<T> createActor(const std::u32string& _name = U"Actor");

		/// @brief T型のComponentを作成する
		/// @tparam T Componentの派生クラス
		/// @param _actor Componentの所有者
		template<class T>
		WeakObject<T> createComponent(const WeakObject<Actor>& _actor);

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

		/// @brief シーンに所属するActorの数を返します
		size_t countActor()const noexcept;

		/// @brief シーンに所属するT型のActorの数を取得します
		/// @tparam T Actorの派生クラス
		template<class T>
		size_t countActor()const noexcept;

		/// @brief シーンに所属するComponentの数を取得します
		size_t countComponent()const noexcept;

		/// @brief シーンに所属するT型のComponentの数を取得します
		/// @tparam T Componentの派生クラス
		template<class T>
		size_t countComponent()const noexcept;

		/// @brief UniqueTagに固有の名前を発行する
		/// @brief 既存の名前を指定した場合は ' #n ' の形で連番になります
		/// @param _name 発行したい名前
		/// @param _tag 名前を設定するUniqueTag
		void securedName(const std::u32string& _name, UniqueTag& _tag);

		/// @brief CommonTagにタグを発行する
		/// @param _name 発光したいタグ
		/// @param _tag タグを設定するCommonTag
		void securedTag(const std::u32string& _name, CommonTag& _tag);

	private:

		SharedObject<Internal::SceneObjectDetail> mDetail;

	};

	template<class T>
	WeakObject<T> SceneObject::createActor(const std::u32string& _name)
	{
		return mDetail->createActor<T>(_name);
	}

	template<class T>
	WeakObject<T> SceneObject::createComponent(const WeakObject<Actor>& _actor)
	{
		return mDetail->createComponent<T>(_actor);
	}

	template<class T>
	size_t SceneObject::countActor()const noexcept
	{
		return mDetail->countActor<T>();
	}

	template<class T>
	size_t SceneObject::countComponent()const noexcept
	{
		return mDetail->countComponent(typeid(T));
	}
}