#pragma once
	
#include <vector>
#include <Framework/Component.hpp>
#include <Classes/Tag.hpp>

namespace eagle
{
	class Actor : Uncopyable
	{
	public:

		explicit Actor();

		virtual ~Actor();

		/// @brief Actorが所属するシーンのオブジェクトを取得する
		ObjectHandle<Internal::SceneObjectDetail> sceneObject()const noexcept;

		/// @brief 名前を設定する 
		void setName(const std::u32string& _name);

		/// @brief タグを追加する
		void addTag(const std::u32string& _tag);

		/// @brief タグを削除する
		void removeTag(const std::u32string& _tag);

		/// @brief 有効・無効を設定する
		void setActive(bool _active)noexcept;

		/// @brief 名前を取得する
		const std::u32string& getName()const;

		/// @brief Actorが持つタグを列挙する
		const std::vector<std::u32string> enumrateTag()const;

		/// @brief 指定した文字列がActorの名前と等しいか 
		bool compareName(const std::u32string& _name)const noexcept;

		/// @brief 指定した文字列と等しいタグをActorが有しているかどうか
		bool compareTag(const std::u32string& _tag)const noexcept;

		/// @brief 有効かどうか
		bool isActive()const noexcept;

		/// @brief 破棄命令が出ているどうか
		bool isPendingKill()const noexcept;

		/// @brief T型のコンポーネントをアタッチする
		template<class T>
		WeakObject<T> attachComponent();

		/// @brief 等しいTypeInfoを持つコンポーネントを取得する
		WeakObject<Component> getComponent(const TypeInfo& _info)const;

		/// @brief T型のコンポーネントを取得する
		template<class T>
		WeakObject<T> getComponent()const;

		/// @brief T型のコンポーネントを削除する
		template<class T>
		void detachComponent();

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

		template<class T, std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
		static SharedObject<T> _InternalCreate();

		static bool _InternalSetup(const WeakObject<Internal::SceneObjectDetail>& scene, const SharedObject<Actor>& actor, const std::u32string& name);

	private:

		/// @brief 自身の弱参照
		WeakObject<Actor> mSelf;

		/// @brief 自身が所属しているSceneの弱参照
		WeakObject<Internal::SceneObjectDetail> mSceneObject;

		/// @brief Actor固有の名前
		UniqueTag mName;

		/// @brief Actorが持つタグのリスト
		std::vector<CommonTag> mTagList;

		/// @brief Actorが有効かどうか
		bool mActive;

		/// @brief 破棄命令が出たかどうか
		bool mPendingKill;

		/// @brief このActorにアタッチされたコンポーネントの配列
		std::vector<WeakObject<Component>> mAttachedComponent;

	};

	template<class T>
	inline WeakObject<T> Actor::attachComponent()
	{
		auto result = Component::_InternalCreate<T>();
		auto component = Cast<Component>(result);
		addComponent(component, typeid(T));
		mAttachedComponent.push_back(component.weak());
		return result.weak();
	}

	template<class T>
	inline WeakObject<T> Actor::getComponent() const
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
	inline void Actor::detachComponent()
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

	template<class T, std::enable_if_t<std::is_base_of_v<Actor, T>, int>>
	inline SharedObject<T> Actor::_InternalCreate()
	{
		return MakeShared<T>();
	}
}