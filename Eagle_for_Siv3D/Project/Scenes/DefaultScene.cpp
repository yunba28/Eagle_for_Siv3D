#include "DefaultScene.hpp"

#include <Components/GUI/Background.hpp>
#include <Components/GUI/FlexibleButton.hpp>
#include <Components/GUI/Text.hpp>
#include <Components/GUI/Slider.hpp>

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
	void draw()const override
	{
		auto pos = getTransform()->getPos2D();
		Circle{ pos,20 }.draw(Palette::Red);
	}
};

DefaultScene::DefaultScene(const InitData& _init)
	: Super(_init)
{
	InputAxis[U"Horizontal"]
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
		.add(XInput(0).buttonB);

	JSON config;
	config[U"InputAxis"] = InputAxis.toJson();
	config[U"InputAction"] = InputAction.toJson();
	config.save(U"config.json");

	{
		using namespace eagle::GUI;

		auto actor = createActor().lock();

		auto transform = actor->getTransform();
		transform->setPos(40, 100);

		auto slider = actor->attachComponent<Slider>().lock();
		slider->setLengthAndThickness(200, 20, SlideDirection::LeftToRight);
		slider->setValueMinMax(0.0, 100.0);
	}

	{
		auto actor = createActor().lock();
		actor->attachComponent<MoveComponent>();
		actor->attachComponent<CircleComponent>();
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
