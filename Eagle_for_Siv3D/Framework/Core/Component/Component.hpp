#pragma once

#include <Core/CoreFwd.hpp>
#include <Utility/SmartObject.hpp>

namespace eagle
{
	class Component : Uncopyable
	{
	public:

		Component();

		virtual ~Component() = 0;

		/*---------------------------------------- 汎用関数 -----------------------------------------------*/

		/// @brief Componentの有効・無効を設定する
		void setEnable(bool _enable)noexcept;

		/// @brief Componentの関数の有効・無効を設定する
		/// @param _update update関数のフラグ
		/// @param _lateUpdate lateUpdate関数のフラグ 
		/// @param _fixedUpdate fixedUpdate関数のフラグ
		void setFunctionEnable(bool _update, bool _lateUpdate, bool _fixedUpdate)noexcept;

		/// @brief アタッチ先のActorの参照を取得
		Actor_handle getActor()const noexcept;

		/// @brief アタッチ先のActorの参照を取得
		template<class T>
		ObjectHandle<T> getActor()const noexcept requires(std::derived_from<T, Actor>);

		/// @brief Actorが所有するTransformの参照を取得する
		Transform_handle getTransform()const noexcept;

		/// @brief Componentの型情報を取得する
		const TypeID& getType()const noexcept;

		/// @brief Componentの型ハッシュを取得する 
		const uint64 getCode()const noexcept;

		/// @brief Componentの型情報が等しいか比較する
		/// @param _type 比較する型情報
		bool compareType(const TypeID& _type)const noexcept;

		/// @brief 文字列がActorの名前と等しいか比較する
		/// @param _name 比較する文字列
		bool compareName(const String& _name)const noexcept;

		/// @brief 文字列がActorが有するいずれかのタグと等しいか
		/// @param _tag 比較する文字列
		bool compareTag(const String& _tag)const noexcept;

		/// @brief Componentが有効かどうか
		bool isEnable()const noexcept;

		/// @brief Componentに破棄命令が出ているか
		bool isPendingKill()const noexcept;

	public:

		/*-------------------------------------- メッセージ関数 ---------------------------------------------*/

		virtual void awake() {}

		virtual void dispose() {}

		virtual void start() {}

		virtual void update() {}

		virtual void lateUpdate() {}

		virtual void fixedUpdate() {}

		virtual void onEnable() {}

		virtual void onDisable() {}

		virtual void onCollisionEnter(const Collision&) {}

		virtual void onCollisionStay(const Collision&) {}

		virtual void onCollisionExit(const Collision&) {}

		virtual void _internalAwake();

		virtual void _internalDispose();

		virtual void _internalUpdate();

		virtual void _internalLateUpdate();

		virtual void _internalFixedUpdate();

	protected:

		/// @brief Componentの有効・無効を設定する
		/// @param _enable Componentの有効・無効
		/// @param _callEvent onEnable・onDisableを呼ぶかどうか
		void setEnable(bool _enable, bool _callEvent);

		/// @brief update関数が有効かどうか
		bool isEnableUpdate()const noexcept;

		/// @brief lateUpdateが有効かどうか
		bool isEnableLateUpdate()const noexcept;

		/// @brief fixedUpdateが有効かどうか
		bool isEnableFixedUpdate()const noexcept;

		/// @brief fixedUpdateが使えるようにする
		void joinFixedSystem();

		/// @brief ColliderをFixedSystemに追加する
		void joinFixedSystem(const WeakObject<class Collider2D>& _collider);

	public:

		/// @brief Componentを作成する
		/// @tparam T 作成するComponentの型
		/// @param actor アタッチする先のActor
		template<class T>
		static SharedObject<T> Create(const Actor_ref& actor)requires(std::derived_from<T, Component>);

		/// @brief Componentを削除する
		static void Destroy(const Component_ref& component);

		/// @brief Componentを削除する
		static void Destroy(const Component_handle& component);

	private:

		static void Create_impl(const Actor_ref& actor, const SharedObject<Component>& component, const std::type_index& type);

	protected:

		/// @brief 自身の参照
		Component_ref mThis;

	private:

		/// @brief 付帯されている先のActorの参照
		Actor_ref mActor;

		/// @brief Componentの型情報
		TypeID mType;

		/// @brief Comopnentの有効・無効
		bool mIsEnabled;

		/// @brief Componentが最初の更新を行ったかどうか
		bool mIsStarted;

		/// @brief Componentが破棄待ちの状態かどうか
		bool mIsPendingKill;

		/// @brief update関数が有効かどうか
		bool mIsEnabledUpdate;

		/// @brief lateUpdate関数が有効かどうか
		bool mIsEnabledLateUpdate;

		/// @brief fixedUpdate関数が有効かどうか
		bool mIsEnabledFixedUpdate;

		/// @brief fixedUpdate関数が使える状態かどうか
		bool mIsJoinedFixedSystem;

	};
}

#include "Component.ipp"
