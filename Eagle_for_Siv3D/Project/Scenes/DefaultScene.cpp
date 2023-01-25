#include "DefaultScene.hpp"

#include <Framework/Actor.hpp>
#include <Framework/Behavior.hpp>
#include <Framework/Renderer2D.hpp>

class MoveComponent : public eagle::Behavior
{
public:

	void update()override
	{
		Vec2 axis
		{
			KeyRight.pressed() - KeyLeft.pressed(),
			KeyDown.pressed() - KeyUp.pressed()
		};

		if (axis.isZero())
			return;

		axis.normalize();

		const double deltaSpeed = 300.0 * Scene::DeltaTime();
		const Vec2 velocity = axis * deltaSpeed;

		getTransform()->translate(velocity.xy0());
	}

};

class CircleComponent : public eagle::Renderer2D
{
public:

	void draw()const override
	{
		auto pos = getTransform()->getPos2D();
		Circle{ pos,20 }.draw();
	}

};

namespace Project
{
	DefaultScene::DefaultScene(const InitData& _init)
		: Super(_init)
	{
	}

	DefaultScene::~DefaultScene()
	{
	}

	void DefaultScene::awake()
	{
		auto actor = createActor(U"Player").lock();
		auto move = actor->attachComponent<MoveComponent>().lock();
		auto circle = actor->attachComponent<CircleComponent>().lock();
	}

	void DefaultScene::dispose()
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
}
