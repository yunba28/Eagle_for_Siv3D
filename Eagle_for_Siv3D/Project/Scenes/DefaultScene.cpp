#include "DefaultScene.hpp"

#include <Components/GUI/Background.hpp>
#include <Components/GUI/FlexibleButton.hpp>
#include <Components/GUI/Text.hpp>
#include <Components/GUI/Slider.hpp>
#include <Components/GUI/InputField.hpp>

class MoveComponent : public eagle::Behavior
{
	void awake()override
	{
		getTransform()->setPos(Scene::CenterF());
	}

	void update()override
	{
		Vec2 axis
		{
			InputAxis(U"Horizontal"),
			-InputAxis(U"Vertical")
		};

		if (axis.isZero())
		{
			return;
		}

		if (axis.lengthSq() > 1)
		{
			axis.normalize();
		}

		const double deltaSpeed = 200.0 * Scene::DeltaTime();
		const Vec2 velocity = axis * deltaSpeed;

		getTransform()->translate(velocity.xy0());
	}
};

class CircleComponent : public eagle::Renderer2D
{
public:

	void setColor(const Color& _color)
	{
		mColor = _color;
	}

private:

	Color mColor{ 255,0,0 };

	void draw()const override
	{
		auto pos = getTransform()->getPos2D();
		Circle{ pos,20 }.draw(mColor);
	}
};

class DestroyTimer : public eagle::Behavior
{
public:

private:

	Stopwatch mStopwatch;

	virtual void start()override
	{
		mStopwatch.restart();
	}

	virtual void update()override
	{
		if (mStopwatch > 1.0s)
		{
			eagle::Actor::Destroy(*getActor());
			setEnable(false);
		}
	}

};

class CircleSpawnButton : public eagle::GUI::Button
{
public:

private:

	virtual void onClicked()override
	{
		Button::onClicked();

		auto actor = getActor();
		auto scene = actor->getSceneObject();

		const auto region = eagle::GUI::Padding{ 20 }.apply(Scene::Rect());

		{
			auto obj = scene->createActor(U"Circle").lock();
			auto tf = obj->getTransform();
			tf->setPos(RandomVec2(region));
			auto circle = obj->attachComponent<CircleComponent>().lock();
			circle->setColor(RandomHSV({ 0,360 }, { 0,1 }, { 0,1 }));
			obj->attachComponent<DestroyTimer>();
		}
	}

};

class DrawText : public eagle::Renderer2D
{
public:

	DrawText()
	{
		setEnable({ true,false,false }, { false,true });
	}

private:

	eagle::WeakObject<eagle::GUI::InputField> mInputField;

	String mViewText;

	void start()override
	{
		mInputField = getActor()->getComponent<eagle::GUI::InputField>();
	}

	void draw()const override {}

	void drawScreen()const override
	{
		if (auto inputField = mInputField.lock(); inputField)
		{
			if (auto opt = inputField->getEvalOpt();opt)
			{
				Print << *opt;
			}
		}
	}
};

DefaultScene::DefaultScene(const InitData& _init)
	: Super(_init)
{
	/*InputAxis[U"Horizontal"]
		.add(KeyA | KeyLeft, -1)
		.add(KeyD | KeyRight, 1)
		.add({ PadState::LThumbX });

	InputAxis[U"Vertical"]
		.add(KeyW | KeyUp, 1)
		.add(KeyS | KeyDown, -1)
		.add({ PadState::LThumbY });

	InputAction[U"Clicked"]
		.add(KeyEnter)
		.add(MouseL)
		.add(XInput(0).buttonA);

	InputAction[U"Pressed"]
		.add(KeyEnter, { eagle::InputState::Pressed })
		.add(MouseL, { eagle::InputState::Pressed })
		.add(XInput(0).buttonA, { eagle::InputState::Pressed });

	InputAction[U"Decide"]
		.add(KeyEnter)
		.add(MouseL)
		.add(XInput(0).buttonA);

	InputAction[U"Cancel"]
		.add(KeyEscape)
		.add(MouseR)
		.add(XInput(0).buttonB);*/

	if (JSON config = JSON::Load(U"data/data/config.json"); config)
	{
		InputAxis.fromJson(config[U"InputAxis"]);
		InputAction.fromJson(config[U"InputAction"]);
	}

	{
		using namespace eagle::GUI;

		auto actor = createActor().lock();

		auto button = actor->attachComponent<CircleSpawnButton>().lock();
		auto text = actor->attachComponent<Text>().lock();
		text->setGUIRect(button->getGUIRect());
		text->setText(U"Spawn");
		text->setFont(Font{ 28,Typeface::Bold });
		text->setColor(Palette::White);
		text->setTextAligne(TextAligne::Center);
	}

	{
		using namespace eagle::GUI;

		auto actor = createActor().lock();

		auto inputField = actor->attachComponent<InputField>().lock();
		inputField->setSize(SizeF{ 240,40 });
		inputField->setAnchor(Vec2{ 0.5,0.05 });
		inputField->setPivot(Vec2{ 0.5,0.0 });
		inputField->setInputType(InputType::String);

		actor->attachComponent<DrawText>();
	}
}

DefaultScene::~DefaultScene()
{
}

void DefaultScene::update()
{
	Super::update();
}

void DefaultScene::draw() const
{
	Super::draw();
}
