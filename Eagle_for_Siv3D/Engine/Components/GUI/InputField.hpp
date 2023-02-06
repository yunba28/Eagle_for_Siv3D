#pragma once

#include <Components/GUI/Detail/Selectable.hpp>
#include <Framework/Renderer2D.hpp>

namespace eagle::GUI
{
	enum class InputType : uint8
	{
		String,
		NumberOnly
	};

	class InputField : public backend::Selectable
	{
	private:

		class TextView;

		struct InputEditState;

	public:

		struct ColorBlock
		{
			Color backgroundNeutral{ 96,96,96,255 };
			Color backgroundHovered{ 96,96,96,255 };
			Color backgroundPressed{ 96,96,96,255 };
			Color backgroundDisable{ 96,96,96,255 };

			Color frameNeutral{ 192,192,192,255 };
			Color frameHovered{ 255,255,255,255 };
			Color frameActive{ 100,155,255,255 };
			Color frameDisable{ 192,192,192,255 };

			Color textView{ 255,255,255,255 };
			Color textActive{ 100,155,255,255 };
		};

	public:

		InputField();

		virtual ~InputField();

		void setDrawOrder(size_t _order);

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		virtual void onSelect()override;

		void setText(const String& _text);

		void setFont(const Font& _font);

		void setColorBlock(const ColorBlock& _colorBlock);

		void setInputType(InputType _inputType)noexcept;

		ObjectHandle<class Background> getBackground()const noexcept;

		ObjectHandle<class Frame> getFrame()const noexcept;

		const String& getText()const noexcept;

		template<class Arithmetic>
		Arithmetic getNumber()const requires(std::is_arithmetic_v<Arithmetic>);

		template<std::integral Int = int32>
		Int getInt()const;

		template<std::floating_point Float = double>
		Float getFloat()const;

		double getEval()const;

		template<class Arithmetic>
		Optional<Arithmetic> getNumberOpt()const requires(std::is_arithmetic_v<Arithmetic>);

		template<std::integral Int = int32>
		Optional<Int> getIntOpt()const;

		template<std::floating_point Float = double>
		Optional<Float> getFloatOpt()const;

		Optional<double> getEvalOpt()const;

		const Font& getFont()const noexcept;

		const ColorBlock& getColorBlock()const noexcept;

		InputType getInputType()const noexcept;

		bool isActiveInput()const noexcept;

		bool changedText()const noexcept;

		bool pressedEnterKey()const noexcept;

		bool pressedTabKey()const noexcept;

	private:

		ObjectHandle<TextView> getTextView()const noexcept;

		void onActiveTextView();

		void onInactiveTextView();

		void updatePaste(InputEditState& _state, const String& _editingText);

		void updateCursorMove(InputEditState& _state);

		static constexpr bool IsXdigitAlpha(const char32_t ch)
		{
			return InRange(ch,U'a',U'f') or InRange(ch,U'A',U'F');
		}

		static constexpr bool IsNumber(const char32_t ch)
		{
			return IsXdigit(ch) or (ch == U'.');
		}

	private:

		virtual void awake()override;

		virtual void start()override;

		virtual void update()override;

		virtual void onEnable()override;

		virtual void onDisable()override;

		virtual void onNeutral()override;

		virtual void onHovered()override;

		virtual void onUnhovered()override;

		virtual void onClicked()override;

	private:

		WeakObject<class Background> mBackground;

		WeakObject<class Frame> mFrame;

		WeakObject<TextView> mTextView;

		ColorBlock mColorBlock;

		InputType mInputType;

	};

	template<class Arithmetic>
	Arithmetic InputField::getNumber() const requires(std::is_arithmetic_v<Arithmetic>)
	{
		if constexpr (std::is_integral_v<Arithmetic>)
		{
			return getInt<Arithmetic>();
		}
		else if constexpr (std::is_floating_point_v<Arithmetic>)
		{
			return getFloat<Arithmetic>();
		}

		//型制約があるのでここには到達しない
	}

	template<std::integral Int>
	Int InputField::getInt()const
	{
		const auto& text = getText();

		if (text.any(IsXdigitAlpha))
		{
			return ParseInt<Int>(text, 16);
		}
		else if (text.starts_with(U'0'))
		{
			return ParseInt<Int>(text, 8);
		}
		else
		{
			return ParseInt<Int>(text);
		}
	}

	template<std::floating_point Float>
	Float InputField::getFloat()const
	{
		return ParseFloat<Float>(getText());
	}

	template<class Arithmetic>
	Optional<Arithmetic> InputField::getNumberOpt()const requires(std::is_arithmetic_v<Arithmetic>)
	{
		if constexpr (std::is_integral_v<Arithmetic>)
		{
			return getIntOPt<Arithmetic>();
		}
		else if constexpr (std::is_floating_point_v<Arithmetic>)
		{
			return getFloatOpt<Arithmetic>();
		}

		//型制約があるのでここには到達しない
	}

	template<std::integral Int>
	Optional<Int> InputField::getIntOpt()const
	{
		const auto& text = getText();

		if (text.any(IsXdigitAlpha))
		{
			return ParseIntOpt<Int>(text, 16);
		}
		else if (text.starts_with(U'0'))
		{
			return ParseIntOpt<Int>(text, 8);
		}
		else
		{
			return ParseIntOpt<Int>(text);
		}
	}

	template<std::floating_point Float>
	Optional<Float> InputField::getFloatOpt()const
	{
		return ParseFloatOpt<Float>(getText());
	}
}
