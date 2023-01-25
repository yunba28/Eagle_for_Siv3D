#pragma once

#include <Framework/Transform.hpp>
#include <Framework/Detail/UniqueTag.hpp>
#include <Framework/Detail/CommonTag.hpp>

namespace eagle
{
	class Actor : Uncopyable
	{
	public:

		explicit Actor();

		virtual ~Actor();

		/// @brief Actorが所属するシーンのオブジェクトを取得する
		ObjectHandle<backend::SceneObjectDetail> getSceneObject()const noexcept;

		/// @brief Actorが所有するTransformの参照を取得する
		ObjectHandle<Transform> getTransform()const noexcept;

		/// @brief 名前を設定する 
		void setName(const String& _name);

		/// @brief タグを追加する
		void addTag(const String& _tag);

		/// @brief タグを削除する
		void removeTag(const String& _tag);

		/// @brief 有効・無効を設定する
		void setActive(bool _active)noexcept;

		/// @brief 名前を取得する
		const String& getName()const;

		/// @brief Actorが持つタグを列挙する
		const Array<String> enumrateTag()const;

		/// @brief 指定した文字列がActorの名前と等しいか 
		bool compareName(const String& _name)const noexcept;

		/// @brief 指定した文字列と等しいタグをActorが有しているかどうか
		bool compareTag(const String& _tag)const noexcept;

		/// @brief 有効かどうか
		bool isActive()const noexcept;

		/// @brief 破棄命令が出ているどうか
		bool isPendingKill()const noexcept;

		/// @brief T型のコンポーネントをアタッチする
		template<class T>
		WeakObject<T> attachComponent()requires(std::derived_from<T, Component>);

		/// @brief 等しいTypeInfoを持つコンポーネントを取得する
		WeakObject<Component> getComponent(const TypeInfo& _info)const;

		/// @brief T型のコンポーネントを取得する
		template<class T>
		WeakObject<T> getComponent()const requires(std::derived_from<T, Component>);

		/// @brief T型のコンポーネントを削除する
		template<class T>
		void detachComponent()requires(std::derived_from<T, Component>);

	public:

		virtual void awake() {}

		virtual void dispose() {}

		virtual void _internalAwake();

		virtual void _internalDispose();

	protected:

		/// @brief 自身の弱参照を取得する
		const WeakObject<Actor>& self()const noexcept;

	private:

		void addComponent(const SharedObject<Component>& _component, const TypeInfo& _type);

	public:

		static void Destroy(Actor& actor);

		static bool _InternalSetup(const WeakObject<backend::SceneObjectDetail>& scene, const SharedObject<Actor>& actor, const String& name);

	private:

		/// @brief 自身の弱参照
		WeakObject<Actor> mSelf;

		/// @brief 自身が所属しているSceneの弱参照
		WeakObject<backend::SceneObjectDetail> mSceneObject;

		/// @brief Actorが所有するTransformの弱参照
		WeakObject<Transform> mTransform;

		/// @brief Actor固有の名前
		UniqueTag mName;

		/// @brief Actorが持つタグのリスト
		Array<CommonTag> mTagList;

		/// @brief Actorが有効かどうか
		bool mActive;

		/// @brief 破棄命令が出たかどうか
		bool mPendingKill;

		/// @brief このActorにアタッチされたコンポーネントの配列
		Array<WeakObject<Component>> mAttachedComponent;

	};

	template<class T>
	inline WeakObject<T> Actor::attachComponent() requires(std::derived_from<T, Component>)
	{
		auto result = MakeShared<T>();
		auto component = Cast<Component>(result);
		addComponent(component, typeid(T));
		mAttachedComponent.push_back(component.weak());
		return result.weak();
	}

	template<class T>
	inline WeakObject<T> Actor::getComponent() const requires(std::derived_from<T, Component>)
	{
		auto find = [type = TypeInfo{ typeid(T) }](const WeakObject<Component>& weak)
		{
			if (auto p = weak.lock(); p)
				return p->getTypeInfo() == type;
			return false;
		};

		auto found = std::find_if(mAttachedComponent.begin(), mAttachedComponent.end(), find);

		if (found != mAttachedComponent.end())
		{
			return Cast<T>(*found);
		}

		return WeakObject<T>{};
	}

	template<class T>
	inline void Actor::detachComponent() requires(std::derived_from<T, Component>)
	{
		auto find = [type = TypeInfo{ typeid(T) }](const WeakObject<Component>& weak)
		{
			if (auto p = weak.lock(); p)
				return p->getTypeInfo() == type;
			return false;
		};

		auto found = std::find_if(mAttachedComponent.begin(), mAttachedComponent.end(), find);

		if (found != mAttachedComponent.end())
		{
			auto p = (*found).lock();
			Component::Destroy(*p);
			mAttachedComponent.erase(found);
		}
	}
}
