#pragma once

#include <string>

#include <Framework/FrameworkFwd.hpp>
#include <Classes/Uncopyable.hpp>
#include <Classes/SmartObject.hpp>

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

			/// @brief update�֐��̗L���E����
			bool update;

			/// @brief lateUpdate�֐��̗L���E����
			bool lateUpdate;

			/// @brief fixedUpdate�֐��̗L���E����
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

			/// @brief Component�̗L���E����
			bool value;

			/// @brief start�֐����Ă΂ꂽ���ǂ���
			bool started;

			/// @brief �j�����߂��o���ꂽ��
			bool pendingKill;

			friend class Component;
		};

	public:

		/// @brief ����:�g���r���A���ȃR���X�g���N�^�ł��邱��
		Component()noexcept;

		virtual ~Component() = default;

		/// @brief Component�̗L���E������ݒ肷��
		void setEnable(bool _enable)noexcept;

		/// @brief update�n�֐��̗L���E������ݒ肷��
		void setEnable(Enable _enable)noexcept;

		/// @brief ���g���A�^�b�`����Ă���Actor�̎Q�Ƃ��擾
		ObjectHandle<Actor> getActor()const noexcept;

		/// @brief ���g���A�^�b�`����Ă���Actor�̎Q�Ƃ��擾
		template<class T, enable_if_t<is_base_of_v<Actor,T>, int> = 0>
		ObjectHandle<T> getActor()const noexcept;

		/// @brief Component�̌^�����擾����
		const TypeInfo& getTypeInfo()const noexcept;

		/// @brief �w�肵�����O��Actor�̖��O�ƈ�v���Ă��邩
		bool compareName(const std::u32string& _name)const;

		/// @brief �w�肵��������Ɠ������^�O��Actor���L���Ă��邩�ǂ���
		bool compareTag(const std::u32string& _tag)const noexcept;

		/// @brief Component���L�����ǂ���
		bool isEnable()const noexcept;

		/// @brief start�֐����Ă΂ꂽ���ǂ���
		bool isStarted()const noexcept;

		/// @brief �j�����߂��o����
		bool isPendingKill()const noexcept;

		/// @brief update�֐����L�����ǂ���
		bool isUpdateEnable()const noexcept;

		/// @brief lateUpdate�֐����L�����ǂ���
		bool isLateUpdateEnable()const noexcept;

		/// @brief fixedUpdate�֐����L�����ǂ���
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

		/// @brief Component�ɔj�����߂��o���܂�
		static void Destroy(Component& component);

		template<class T, enable_if_t<is_base_of_v<Component, T>, int> = 0>
		static SharedObject<T> _InternalCreate();

		static bool _InternalSetup(const WeakObject<Actor>& actor, const SharedObject<Component>& component, const TypeInfo& type);

	protected:

		/// @brief ���g�̎�Q�Ƃ��擾����
		const WeakObject<Component>& self()const noexcept;

	private:

		/// @brief ���g�̎�Q��
		WeakObject<Component> mSelf;

		/// @brief ���g���A�^�b�`����Ă���Actor�̎�Q��
		WeakObject<Actor> mActor;

		/// @brief Component�̌^���
		TypeInfo mTypeInfo;

		/// @brief Component�̊e�t���O
		Enable mEnable;

	};

	template<class T, enable_if_t<is_base_of_v<Actor, T>, int>>
	inline ObjectHandle<T> Component::getActor()const noexcept
	{
		return Cast<T>(mActor).lock();
	}

	template<class T, enable_if_t<is_base_of_v<Component, T>, int>>
	inline SharedObject<T> Component::_InternalCreate()
	{
		return MakeShared<T>();
	}
}