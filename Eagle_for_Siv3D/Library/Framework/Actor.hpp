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

		/// @brief Actor����������V�[���̃I�u�W�F�N�g���擾����
		ObjectHandle<Internal::SceneObjectDetail> sceneObject()const noexcept;

		/// @brief ���O��ݒ肷�� 
		void setName(const std::u32string& _name);

		/// @brief �^�O��ǉ�����
		void addTag(const std::u32string& _tag);

		/// @brief �^�O���폜����
		void removeTag(const std::u32string& _tag);

		/// @brief �L���E������ݒ肷��
		void setActive(bool _active)noexcept;

		/// @brief ���O���擾����
		const std::u32string& getName()const;

		/// @brief Actor�����^�O��񋓂���
		const std::vector<std::u32string> enumrateTag()const;

		/// @brief �w�肵��������Actor�̖��O�Ɠ������� 
		bool compareName(const std::u32string& _name)const noexcept;

		/// @brief �w�肵��������Ɠ������^�O��Actor���L���Ă��邩�ǂ���
		bool compareTag(const std::u32string& _tag)const noexcept;

		/// @brief �L�����ǂ���
		bool isActive()const noexcept;

		/// @brief �j�����߂��o�Ă���ǂ���
		bool isPendingKill()const noexcept;

		/// @brief T�^�̃R���|�[�l���g���A�^�b�`����
		template<class T>
		WeakObject<T> attachComponent();

		/// @brief ������TypeInfo�����R���|�[�l���g���擾����
		WeakObject<Component> getComponent(const TypeInfo& _info)const;

		/// @brief T�^�̃R���|�[�l���g���擾����
		template<class T>
		WeakObject<T> getComponent()const;

		/// @brief T�^�̃R���|�[�l���g���폜����
		template<class T>
		void detachComponent();

	public:

		virtual void awake() {}

		virtual void dispose() {}

		virtual void _internalAwake();

		virtual void _internalDispose();

	protected:

		/// @brief ���g�̎�Q�Ƃ��擾����
		const WeakObject<Actor>& self()const noexcept;

	private:

		void addComponent(const SharedObject<Component>& _component, const TypeInfo& _type);

	public:

		static void Destroy(Actor& actor);

		template<class T, std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
		static SharedObject<T> _InternalCreate();

		static bool _InternalSetup(const WeakObject<Internal::SceneObjectDetail>& scene, const SharedObject<Actor>& actor, const std::u32string& name);

	private:

		/// @brief ���g�̎�Q��
		WeakObject<Actor> mSelf;

		/// @brief ���g���������Ă���Scene�̎�Q��
		WeakObject<Internal::SceneObjectDetail> mSceneObject;

		/// @brief Actor�ŗL�̖��O
		UniqueTag mName;

		/// @brief Actor�����^�O�̃��X�g
		std::vector<CommonTag> mTagList;

		/// @brief Actor���L�����ǂ���
		bool mActive;

		/// @brief �j�����߂��o�����ǂ���
		bool mPendingKill;

		/// @brief ����Actor�ɃA�^�b�`���ꂽ�R���|�[�l���g�̔z��
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