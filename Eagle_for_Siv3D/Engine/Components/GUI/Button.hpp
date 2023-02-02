#pragma once

#include <Components/GUI/Detail/Selectable.hpp>

namespace eagle::GUI
{
	class Button : public backend::Selectable
	{
	public:

		struct ColorBlock
		{
			Color neutral{ 255,255,255,64 };
			Color hovered{ 255,255,255,128 };
			Color pressed{ 192,192,192,128 };
			Color disable{ 64,64,64,64 };
		};

		struct TextureBlock
		{
			Texture neutral{};
			Texture hovered{};
			Texture pressed{};
			Texture disable{};
		};

		using Super = Button;

	public:

		Button();

		virtual ~Button();

		void setDrawOrder(size_t _order);

		virtual void setGUIRect(const GUIRect& _GUIRect)override;

		virtual void setRegion(RectF _region)override;

		virtual void setSize(SizeF _size)override;

		virtual void setAnchor(Vec2 _anchor)override;

		virtual void setPivot(Vec2 _pivot)override;

		void setColorBlock(const ColorBlock& _colorBlock);

		void setTextureBlock(const Optional<TextureBlock>& _textureBlock);

		ObjectHandle<class Background> getBackground()const noexcept;

		const ColorBlock& getColorBlock()const noexcept;

		const Optional<TextureBlock>& getTextureBlock()const noexcept;

	protected:

		virtual void awake()override;

		virtual void start()override;

		virtual void onEnable()override;

		virtual void onDisable()override;

		virtual void onHovered()override;

		virtual void onUnhovered()override;

		virtual void onClicked()override;

		virtual void onReleased()override;

	private:

		WeakObject<class Background> mBackground;

		ColorBlock mColorBlock;

		Optional<TextureBlock> mTextureBlock;

	};
}
