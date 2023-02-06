#include <Components/GUI/InputField.hpp>

#include <Framework/Actor.hpp>

/// @brief Renderer
namespace eagle::GUI
{
	struct InputField::InputEditState
	{
		String text{ U"" };

		size_t cursorIndex{ 0 };

		size_t maxCharCount{ UINT64_MAX };

		bool activeInput{ false };

		bool changeText{ false };

		bool pressEnter{ false };

		bool pressTab{ false };

		Stopwatch cursorStopwatch{};

		Stopwatch leftPressStopwatch{};

		Stopwatch rightPressStopwatch{};
	};

	class InputField::TextView final : public Renderer2D, public IWidget
	{
	public:

		static constexpr double TextOffsetX = 8.0;

		static constexpr double TextOffsetY = -1.0;

	public:

		TextView()
			: mState()
			, mFont(32)
		{
			setEnable({ false,false,false }, { false,true });
		}

		~TextView()
		{

		}

		InputEditState& state()noexcept
		{
			return mState;
		}

		const InputEditState& state()const noexcept
		{
			return mState;
		}

		void setGUIRect(const GUIRect& _GUIRect) override
		{
			mGUIRect = _GUIRect;
		}

		void setRegion(RectF _region) override
		{
			mGUIRect.region = _region;
		}

		void setSize(SizeF _size) override
		{
			mGUIRect.size = _size;
		}

		void setAnchor(Vec2 _anchor) override
		{
			mGUIRect.anchor = _anchor;
		}

		void setPivot(Vec2 _pivot) override
		{
			mGUIRect.pivot = _pivot;
		}

		void setFont(const Font& _font) noexcept
		{
			mFont = _font;
		}
		
		void setColor(const Color& _color) noexcept
		{
			mColor = _color;
		}

		const GUIRect& getGUIRect()const noexcept
		{
			return mGUIRect;
		}

		const Font& getFont()const noexcept
		{
			return mFont;
		}

		const Color& getColor()const noexcept
		{
			return mColor;
		}

	private:

		void drawActive(const RectF& _viewRect)const
		{

			const String editingText = ((mState.activeInput) ? TextInput::GetEditingText() : U"");

			const String textHeader = mState.text.substr(0, mState.cursorIndex);
			const String textTail = mState.text.substr(mState.cursorIndex, String::npos);

			#if SIV3D_PLATFORM(WINDOWS)
				const auto [editingCursorIndex, editingTargetlength] = Platform::Windows::TextInput::GetCursorIndex();
				const bool hasEditingTarget = (editingTargetlength > 0);
			#else
				const int32 editingCursorIndex = -1, editingTargetlength = 0;
				const bool hasEditingTarget = false;
			#endif

			const double fontHeight = mFont.height();
			Vec2 textPos{ _viewRect.x + TextOffsetX,_viewRect.centerY() - mFont.height() / 2 + TextOffsetY - 0.5 };
			double cursorPosX = 0.0;

			{
				double begX = 0.0, begY = 0.0, endX = 0.0;
				{
					ScopedCustomShader2D shader{ Font::GetPixelShader(mFont.method()) };

					for (const auto& glyph : mFont.getGlyphs(textHeader))
					{
						glyph.texture.draw((textPos + glyph.getOffset()), mColor);
						textPos.x += glyph.xAdvance;
					}

					begX = textPos.x;
					begY = 0;
					endX = textPos.x;

					for (auto&& [i, glyph] : Indexed(mFont.getGlyphs(editingText)))
					{
						const int32 currentCharIndex = static_cast<int32>(i);

						if (currentCharIndex == editingCursorIndex)
						{
							begX = textPos.x;
							begY = textPos.y + fontHeight;
						}
						else if (hasEditingTarget && (currentCharIndex == (editingCursorIndex + editingTargetlength - 1)))
						{
							endX = textPos.x + glyph.xAdvance;
						}

						glyph.texture.draw((textPos + glyph.getOffset()), mColor);
						textPos.x += glyph.xAdvance;
					}

					cursorPosX = textPos.x;

					for (const auto& glyph : mFont.getGlyphs(textTail))
					{
						glyph.texture.draw((textPos + glyph.getOffset()), mColor);
						textPos.x += glyph.xAdvance;
					}
				}

				if (hasEditingTarget)
				{
					Line(begX, begY, endX, begY).movedBy(0, -2).draw(2, mColor);
				}
			}

			const bool showCursor = (mState.cursorStopwatch.ms() % 1200 < 600)
				|| (mState.leftPressStopwatch.isRunning() && mState.leftPressStopwatch < 0.5s)
				|| (mState.rightPressStopwatch.isRunning() && mState.rightPressStopwatch < 0.5s);

			if (showCursor)
			{
				const RectF cursor(Arg::leftCenter(Vec2(cursorPosX, _viewRect.centerY()).asPoint()), 1, mFont.fontSize());
				cursor.draw(mColor);
			}
		}

	private:

		void draw()const override final {}

		void drawScreen()const override final
		{
			auto transform = getTransform();
			auto pos = transform->getPos2D();
			auto scale = transform->getScale2D();

			const auto viewRect = mGUIRect.getViewRect(pos, scale);

			if (mState.activeInput)
			{
				drawActive(viewRect);
			}
			else
			{
				const Vec2 textPos{ viewRect.x + TextOffsetX, viewRect.centerY() - mFont.height() / 2 + TextOffsetY - 0.5 };
				mFont(mState.text).draw(textPos, mColor);
			}
		}

	private:

		InputEditState mState;

		Font mFont;

		Color mColor;
	};

}

#include <Components/GUI/Background.hpp>
#include <Components/GUI/Frame.hpp>
#include <Common/InputAction.hpp>

/// @brief Behavior
namespace eagle::GUI
{
	InputField::InputField()
		: mBackground()
		, mFrame()
		, mTextView()
		, mColorBlock()
		, mInputType(InputType::String)
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
		getTextView()->setGUIRect(_GUIRect);
		getFrame()->setGUIRect(_GUIRect);
	}

	void InputField::setRegion(RectF _region)
	{
		Selectable::setRegion(_region);
		getBackground()->setRegion(_region);
		getTextView()->setRegion(_region);
		getFrame()->setRegion(_region);
	}

	void InputField::setSize(SizeF _size)
	{
		Selectable::setSize(_size);
		getBackground()->setSize(_size);
		getTextView()->setSize(_size);
		getFrame()->setSize(_size);
	}

	void InputField::setAnchor(Vec2 _anchor)
	{
		Selectable::setAnchor(_anchor);
		getBackground()->setAnchor(_anchor);
		getTextView()->setAnchor(_anchor);
		getFrame()->setAnchor(_anchor);
	}

	void InputField::setPivot(Vec2 _pivot)
	{
		Selectable::setPivot(_pivot);
		getBackground()->setPivot(_pivot);
		getTextView()->setPivot(_pivot);
		getFrame()->setPivot(_pivot);
	}

	void InputField::onSelect()
	{
		Selectable::onSelect();
		getTextView()->setColor(mColorBlock.textActive);
	}

	void InputField::setText(const String& _text)
	{
		getTextView()->state().text = _text;
	}

	void InputField::setFont(const Font& _font)
	{
		getTextView()->setFont(_font);
	}

	void InputField::setColorBlock(const ColorBlock& _colorBlock)
	{
		mColorBlock = _colorBlock;
		getTextView()->setColor(_colorBlock.textView);

		auto textView = getTextView();
		auto& state = textView->state();
		textView->setColor(state.activeInput ? mColorBlock.textActive : mColorBlock.textView);
	}

	void InputField::setInputType(InputType _inputType) noexcept
	{
		mInputType = _inputType;
	}

	ObjectHandle<class Background> InputField::getBackground() const noexcept
	{
		return mBackground.lock();
	}

	ObjectHandle<class Frame> InputField::getFrame() const noexcept
	{
		return mFrame.lock();
	}

	const String& InputField::getText() const noexcept
	{
		return getTextView()->state().text;
	}

	double InputField::getEval() const
	{
		return Eval(getTextView()->state().text);
	}

	Optional<double> InputField::getEvalOpt() const
	{
		return EvalOpt(getTextView()->state().text);
	}

	const Font& InputField::getFont() const noexcept
	{
		return getTextView()->getFont();
	}

	const InputField::ColorBlock& InputField::getColorBlock() const noexcept
	{
		return mColorBlock;
	}

	InputType InputField::getInputType() const noexcept
	{
		return mInputType;
	}

	bool InputField::isActiveInput() const noexcept
	{
		return getTextView()->state().activeInput;
	}

	bool InputField::changedText() const noexcept
	{
		return getTextView()->state().changeText;
	}

	bool InputField::pressedEnterKey() const noexcept
	{
		return getTextView()->state().pressEnter;
	}

	bool InputField::pressedTabKey() const noexcept
	{
		return getTextView()->state().pressTab;
	}

	ObjectHandle<InputField::TextView> InputField::getTextView() const noexcept
	{
		return mTextView.lock();
	}

	void InputField::onActiveTextView()
	{
		auto textView = getTextView();
		textView->state().activeInput = true;
		textView->setColor(mColorBlock.textActive);
		getFrame()->setColor(mColorBlock.frameActive);
	}

	void InputField::onInactiveTextView()
	{
		auto textView = getTextView();
		textView->state().activeInput = false;
		textView->setColor(mColorBlock.textView);
		getFrame()->setColor(mColorBlock.frameNeutral);

		if (mInputType == InputType::NumberOnly)
		{
			auto& text = textView->state().text;

			// 文字列が空の倍は処理しない
			if (not text.empty())
			{
				// 先頭に符号が付いているかどうか
				auto isSigned = text.starts_with(U'+') or text.starts_with(U'-');
				// 式の可能性があるか
				auto chanceEval = not String{ text.begin() + 1,text.end() }.all(IsNumber);

				if (chanceEval)
				{
					// 式として処理できれば結果を文字列に反映する
					if (auto opt = EvalOpt(text); opt)
					{
						text = ToString(*opt);
					}
					// 失敗した場合はクリアできる
					else
					{
						text.clear();
					}
				}
				// 先頭に符号が付かず数値以外が入っている場合はクリアする
				else if (not (isSigned or IsDigit(text[0])))
				{
					text.clear();
				}
			}
		}
	}

	void InputField::updatePaste(InputEditState& _state, const String& _editingText)
	{
		_state.cursorIndex = TextInput::UpdateText(_state.text, _state.cursorIndex, TextInputMode::AllowBackSpaceDelete);

		if ((not _editingText) &&
		#if SIV3D_PLATFORM(MACOS)
			((KeyCommand + KeyV).down() || (KeyControl + KeyV).down()))
		#else
			(KeyControl + KeyV).down())
		#endif
		{
			if (String paste; Clipboard::GetText(paste))
			{
				_state.text.insert(_state.cursorIndex, paste);
				_state.cursorIndex += paste.size();
			}
		}
	}

	void InputField::updateCursorMove(InputEditState& _state)
	{
		// [←] キー
		if ((0 < _state.cursorIndex)
			&& (KeyLeft.down() || ((0.33s < KeyLeft.pressedDuration()) && (SecondsF{ 0.06 } < _state.leftPressStopwatch))))
		{
			--_state.cursorIndex;
			_state.leftPressStopwatch.restart();
		}

		// [→] キー
		if ((_state.cursorIndex < _state.text.size())
			&& (KeyRight.down() || ((0.33s < KeyRight.pressedDuration()) && (SecondsF{ 0.06 } < _state.rightPressStopwatch))))
		{
			++_state.cursorIndex;
			_state.rightPressStopwatch.restart();
		}
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
		background->setGUIRect(mGUIRect);

		mTextView = actor->attachComponent<TextView>();
		auto textView = mTextView.lock();
		textView->setGUIRect(mGUIRect);

		mFrame = actor->attachComponent<Frame>();
		auto frame = mFrame.lock();
		frame->setGUIRect(mGUIRect);
	}

	void InputField::start()
	{
		getBackground()->setColor(mColorBlock.backgroundNeutral);
		getFrame()->setColor(mColorBlock.frameNeutral);
		getTextView()->setColor(mColorBlock.textView);
	}

	void InputField::update()
	{
		Selectable::update();

		auto& state = getTextView()->state();

		const String previousText = state.text;

		if (state.activeInput)
		{
			const String editingText = TextInput::GetEditingText();

			// ショートカットキーによるペースト
			updatePaste(state, editingText);

			// [←][→] キーでテキストカーソルを移動
			// 一定時間押下すると、テキストカーソルが高速に移動
			if (not editingText)
			{
				updateCursorMove(state);
			}

			// 最大字数を超えていたら削る
			if (state.maxCharCount < state.text.size())
			{
				state.text.resize(state.maxCharCount);
				state.cursorIndex = Min(state.cursorIndex, state.maxCharCount);
			}

			// 文字列に変更があればカーソル点滅をリセット
			if (state.text != previousText)
			{
				state.cursorStopwatch.restart();
			}

			// [home] キーでテキストカーソルを先頭へ移動
			if (KeyHome.down())
			{
				state.cursorIndex = 0;
				state.cursorStopwatch.restart();
			}

			// [end] キーでテキストカーソルを末尾へ移動
			if (KeyEnd.down())
			{
				state.cursorIndex = state.text.length();
				state.cursorStopwatch.restart();
			}

			// [tab] キーで入力カーソルを非アクティブに
			{
				const String raw = TextInput::GetRawInput();
				state.pressTab = raw.includes(U'\t');
				state.pressEnter = raw.includes(U'\r');

				if (state.pressTab || state.pressEnter)
				{
					onInactiveTextView();
				}
			}
		}

		state.changeText = (state.text != previousText);
	}

	void InputField::onEnable()
	{
		getBackground()->setColor(mColorBlock.backgroundNeutral);
		getFrame()->setColor(mColorBlock.frameNeutral);
	}

	void InputField::onDisable()
	{
		getBackground()->setColor(mColorBlock.backgroundDisable);
		getFrame()->setColor(mColorBlock.frameDisable);
		onInactiveTextView();
	}

	void InputField::onNeutral()
	{
		if (MouseL.down())
		{
			onInactiveTextView();
		}
	}

	void InputField::onHovered()
	{
		getBackground()->setColor(mColorBlock.backgroundHovered);

		if (not getTextView()->state().activeInput)
		{
			getFrame()->setColor(mColorBlock.frameHovered);
		}

		Cursor::RequestStyle(CursorStyle::IBeam);
	}

	void InputField::onUnhovered()
	{
		getBackground()->setColor(mColorBlock.backgroundNeutral);

		if (not getTextView()->state().activeInput)
		{
			getFrame()->setColor(mColorBlock.frameNeutral);
		}
	}

	void InputField::onClicked()
	{
		getBackground()->setColor(mColorBlock.backgroundPressed);

		onActiveTextView();

		auto& state = getTextView()->state();

		const auto& font = getTextView()->getFont();

		const auto transform = getTransform();
		const auto pos = transform->getPos2D();
		const auto scale = transform->getScale2D();

		const auto viewRect = mGUIRect.getViewRect(pos, scale);

		const double posX = Cursor::PosF().x - (viewRect.x + TextView::TextOffsetX);

		size_t index = 0;
		double textPos = 0.0;

		for (const auto& advance : font(state.text).getXAdvances())
		{
			if (posX <= (textPos + (advance / 2)))
			{
				break;
			}

			textPos += advance;
			++index;
		}

		state.cursorIndex = index;
		state.cursorStopwatch.restart();
		state.leftPressStopwatch.reset();
		state.rightPressStopwatch.reset();
	}
}
