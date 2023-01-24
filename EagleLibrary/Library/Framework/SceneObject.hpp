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

			/// @brief T�^��Actor���쐬����
			/// @tparam T Actor�̔h���N���X
			/// @param _name Actor�̖��O
			template<class T = Actor>
			WeakObject<T> createActor(const std::u32string& _name = U"Actor");

			/// @brief T�^��Component���쐬����
			/// @tparam T Component�̔h���N���X
			/// @param _actor Component�̏��L��
			template<class T>
			WeakObject<T> createComponent(const WeakObject<Actor>& _actor);

			/// @brief ��������Actor���V�[���ɒǉ�����
			/// @brief *���[�U�[����g�����Ƃ͒ʏ킠��܂���
			/// @param _actor �ǉ�����Actor
			/// @param _type �ǉ�����Actor�̌^���
			void addActor(const SharedObject<Actor>& _actor, const TypeInfo& _type);

			/// @brief ��������Component���V�[���ɒǉ�����
			/// @brief *���[�U�[����g�����Ƃ͒ʏ킠��܂���
			/// @param _component �ǉ�����Component
			/// @param _type �ǉ�����Component�̌^���
			void addComponent(const SharedObject<Component>& _component, const TypeInfo& _type);

			/// @brief �V�[���ɏ�������Actor�̐���Ԃ��܂�
			size_t countActor()const noexcept;

			/// @brief �V�[���ɏ�������T�^��Actor�̐����擾���܂�
			/// @tparam T Actor�̔h���N���X
			template<class T, std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
			size_t countActor()const noexcept;

			/// @brief �V�[���ɏ�������Component�̐����擾���܂�
			size_t countComponent()const noexcept;

			/// @brief �V�[���ɏ�������T�^��Component�̐����擾���܂�
			/// @tparam T Component�̔h���N���X
			template<class T, std::enable_if_t<std::is_base_of_v<Component, T>, int> = 0>
			size_t countComponent()const noexcept;

			/// @brief UniqueTag�ɌŗL�̖��O�𔭍s����
			/// @brief �����̖��O���w�肵���ꍇ�� ' #n ' �̌`�ŘA�ԂɂȂ�܂�
			/// @param _name ���s���������O
			/// @param _tag ���O��ݒ肷��UniqueTag
			void securedName(const std::u32string& _name, UniqueTag& _tag);

			/// @brief CommonTag�Ƀ^�O�𔭍s����
			/// @param _name �����������^�O
			/// @param _tag �^�O��ݒ肷��CommonTag
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

		/// @brief �V�[�����Ǘ�����Actor��Component�̍X�V���s��
		void update();

		/// @brief T�^��Actor���쐬����
		/// @tparam T Actor�̔h���N���X
		/// @param _name Actor�̖��O
		template<class T = Actor>
		WeakObject<T> createActor(const std::u32string& _name = U"Actor");

		/// @brief T�^��Component���쐬����
		/// @tparam T Component�̔h���N���X
		/// @param _actor Component�̏��L��
		template<class T>
		WeakObject<T> createComponent(const WeakObject<Actor>& _actor);

		/// @brief ��������Actor���V�[���ɒǉ�����
		/// @brief *���[�U�[����g�����Ƃ͒ʏ킠��܂���
		/// @param _actor �ǉ�����Actor
		/// @param _type �ǉ�����Actor�̌^���
		void addActor(const SharedObject<Actor>& _actor, const TypeInfo& _type);

		/// @brief ��������Component���V�[���ɒǉ�����
		/// @brief *���[�U�[����g�����Ƃ͒ʏ킠��܂���
		/// @param _component �ǉ�����Component
		/// @param _type �ǉ�����Component�̌^���
		void addComponent(const SharedObject<Component>& _component, const TypeInfo& _type);

		/// @brief �V�[���ɏ�������Actor�̐���Ԃ��܂�
		size_t countActor()const noexcept;

		/// @brief �V�[���ɏ�������T�^��Actor�̐����擾���܂�
		/// @tparam T Actor�̔h���N���X
		template<class T>
		size_t countActor()const noexcept;

		/// @brief �V�[���ɏ�������Component�̐����擾���܂�
		size_t countComponent()const noexcept;

		/// @brief �V�[���ɏ�������T�^��Component�̐����擾���܂�
		/// @tparam T Component�̔h���N���X
		template<class T>
		size_t countComponent()const noexcept;

		/// @brief UniqueTag�ɌŗL�̖��O�𔭍s����
		/// @brief �����̖��O���w�肵���ꍇ�� ' #n ' �̌`�ŘA�ԂɂȂ�܂�
		/// @param _name ���s���������O
		/// @param _tag ���O��ݒ肷��UniqueTag
		void securedName(const std::u32string& _name, UniqueTag& _tag);

		/// @brief CommonTag�Ƀ^�O�𔭍s����
		/// @param _name �����������^�O
		/// @param _tag �^�O��ݒ肷��CommonTag
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