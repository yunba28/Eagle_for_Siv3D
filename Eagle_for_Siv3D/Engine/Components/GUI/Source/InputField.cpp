#include <Components/GUI/InputField.hpp>

#include <Framework/Actor.hpp>

/// @brief Renderer
namespace eagle::GUI
{
	InputField::Renderer::Renderer()
		: edit()
		, font(24)
	{

	}

	InputField::Renderer::~Renderer()
	{

	}

	void InputField::Edit::clear() noexcept
	{
		text.clear();
		cursorPos = 0;
		leftPressStopwatch.reset();
		rightPressStopwatch.reset();
		cursorStopwatch.reset();
	}

	InputField::Renderer::Renderer()
	{
	}

	InputField::Renderer::~Renderer()
	{
	}

	void InputField::Renderer::drawScreen() const
	{

	}

}

#include <Components/GUI/Background.hpp>

/// @brief Behavior
namespace eagle::GUI
{
	InputField::InputField()
	{
	}

	InputField::~InputField()
	{
	}

	void InputField::setDrawOrder(size_t _order)
	{
		getBackground()->setDrawOrder(_order);
	}

	void InputField::setGUIRect(const GUIRect& _GUIRect)
	{
		Selectable::setGUIRect(_GUIRect);
		getBackground()->setGUIRect(_GUIRect);
	}

	void InputField::setRegion(RectF _region)
	{
		Selectable::setRegion(_region);
		getBackground()->setRegion(_region);
	}

	void InputField::setSize(SizeF _size)
	{
		Selectable::setSize(_size);
		getBackground()->setSize(_size);
	}

	void InputField::setAnchor(Vec2 _anchor)
	{
		Selectable::setAnchor(_anchor);
		getBackground()->setAnchor(_anchor);
	}

	void InputField::setPivot(Vec2 _pivot)
	{
		Selectable::setPivot(_pivot);
		getBackground()->setPivot(_pivot);
	}

	ObjectHandle<class Background> InputField::getBackground() const noexcept
	{
		return mBackground.lock();
	}

	ObjectHandle<InputField::Renderer> InputField::getRenderer() const noexcept
	{
		return mRenderer.lock();
	}

	void InputField::awake()
	{
		mGUIRect.size.set(200, 40);

		auto actor = getActor();

		if (mBackground = actor->getComponent<Background>(); mBackground.expired())
		{
			mBackground = actor->attachComponent<Background>();
		}

		auto background = mBackground.lock();
		background->setGUIRect(getGUIRect());

		mRenderer = actor->attachComponent<Renderer>();
		auto renderer = mRenderer.lock();
	}

	void InputField::update()
	{
		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		auto& edit = getRenderer()->edit;
		edit.cursorPos = Min(edit.cursorPos, edit.text.size());
		edit.tabKey = false;
		edit.enterKey = false;

		
	}

	void InputField::onEnable()
	{
		getBackground()->setColor(mColorBlock.neutral);
		getRenderer()->setEnable(true);
	}

	void InputField::onDisable()
	{
		getBackground()->setColor(mColorBlock.disable);
		getRenderer()->setEnable(false);
	}

	void InputField::onHovered()
	{
		getBackground()->setColor(mColorBlock.hovered);
	}

	void InputField::onUnhovered()
	{
		getBackground()->setColor(mColorBlock.neutral);
	}

	void InputField::onClicked()
	{
		getBackground()->setColor(mColorBlock.pressed);
	}
}
