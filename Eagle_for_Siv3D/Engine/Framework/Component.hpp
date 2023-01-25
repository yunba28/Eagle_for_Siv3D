#pragma once

#include <Framework/FrameworkFwd.hpp>
#include <Common/SmartObject.hpp>

namespace eagle
{
	class Component : Uncopyable
	{
	public:

		struct Enable
		{
			constexpr Enable() noexcept
				: update(true)
				, lateUpdate(true)
				, fixedUpdate(false)
				, value(true)
				, started(false)
				, pendingKill(false)
			{}

			constexpr Enable(bool _update, bool _late, bool _fixed) noexcept
				: update(_update)
				, lateUpdate(_late)
				, fixedUpdate(_fixed)
				, value(true)
				, started(false)
				, pendingKill(false)
			{}

			/// @brief update関数の有効・無効
			bool update;

			/// @brief lateUpdate関数の有効・無効
			bool lateUpdate;

			/// @brief fixedUpdate関数の有効・無効
			bool fixedUpdate;

		private:
			constexpr Enable(bool _enable) noexcept
				: update(true)
				, lateUpdate(true)
				, fixedUpdate(false)
				, value(_enable)
				, started(false)
				, pendingKill(false)
			{}

			constexpr Enable(bool _update, bool _late, bool _fixed, bool _enable) noexcept
				: update(_update)
				, lateUpdate(_late)
				, fixedUpdate(_fixed)
				, value(_enable)
				, started(false)
				, pendingKill(false)
			{}

			/// @brief Componentの有効・無効
			bool value;

			/// @brief start関数が呼ばれたかどうか
			bool started;

			/// @brief 破棄命令が出されたか
			bool pendingKill;

			friend class Component;
		};

	public:

		/// @brief 制約:トリビュアルなコンストラクタであること
		Component()noexcept;

		virtual ~Component() = default;

		/// @brief Componentの有効・無効を設定する
		void setEnable(bool _enable)noexcept;

		/// @brief update系関数の有効・無効を設定する
		void setEnable(Enable _enable)noexcept;

		/// @brief 自身がアタッチされているActorの参照を取得
		ObjectHandle<Actor> getActor()const noexcept;

		/// @brief 自身がアタッチされているActorの参照を取得
		template<class T>
		ObjectHandle<T> getActor()const noexcept requires(std::derived_from<T, Actor>);

		/// @brief Actorが所有するTransformの参照を取得
		ObjectHandle<Transform> getTransform()const noexcept;

		/// @brief Componentの型情報を取得する
		const TypeInfo& getTypeInfo()const noexcept;

		/// @brief 指定した名前がActorの名前と一致しているか
		bool compareName(const String& _name)const;

		/// @brief 指定した文字列と等しいタグをActorが有しているかどうか
		bool compareTag(const String& _tag)const noexcept;

		/// @brief Componentが有効かどうか
		bool isEnable()const noexcept;

		/// @brief start関数が呼ばれたかどうか
		bool isStarted()const noexcept;

		/// @brief 破棄命令が出たか
		bool isPendingKill()const noexcept;

		/// @brief update関数が有効かどうか
		bool isUpdateEnable()const noexcept;

		/// @brief lateUpdate関数が有効かどうか
		bool isLateUpdateEnable()const noexcept;

		/// @brief fixedUpdate関数が有効かどうか
		bool isFixedUpdateEnable()const noexcept;

	public:

		virtual void awake() = 0;

		virtual void dispose() = 0;

		virtual void start() = 0;

		virtual void update() = 0;

		virtual void lateUpdate() = 0;

		virtual void fixedUpdate() = 0;

		virtual void onEnable() = 0;

		virtual void onDisable() = 0;

		virtual void _internalAwake();

		virtual void _internalDispose();

		virtual void _internalUpdate();

		virtual void _internalLateUpdate();

		virtual void _internalFixedUpdate();

	public:

		/// @brief Componentに破棄命令を出します
		static void Destroy(Component& component);

		static bool _InternalSetup(const WeakObject<Actor>& actor, const SharedObject<Component>& component, const TypeInfo& type);

	protected:

		/// @brief 自身の弱参照を取得する
		const WeakObject<Component>& self()const noexcept;

	private:

		/// @brief 自身の弱参照
		WeakObject<Component> mSelf;

		/// @brief 自身がアタッチされているActorの弱参照
		WeakObject<Actor> mActor;

		/// @brief Actorが所有するTransformの弱参照
		WeakObject<Transform> mTransform;

		/// @brief Componentの型情報
		TypeInfo mTypeInfo;

		/// @brief Componentの各フラグ
		Enable mEnable;

	};

	template<class T>
	inline ObjectHandle<T> Component::getActor()const noexcept requires(std::derived_from<T, Actor>)
	{
		return mActor.lock<T>();
	}
}
