#include "DefaultScene.hpp"

namespace Project
{
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
				InputAxis(U"MoveX"),
				InputAxis(U"MoveY")
			};

			if (axis.isZero())
			{
				Print << U"Axis:{:-5.3f}"_fmt(axis);
				return;
			}

			if (axis.lengthSq() > 1)
			{
				axis.normalize();
			}

			Print << U"Axis:{:-5.3f}"_fmt(axis);

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
			Circle{ pos,20 }.draw();
		}
	};
}

namespace Project
{
	DefaultScene::DefaultScene(const InitData& _init)
		: Super(_init)
	{
		InputAxis[U"MoveX"]
			.add(KeyA | KeyLeft, -1)
			.add(KeyD | KeyRight, 1)
			.add({ PadState::LThumbX });

		InputAxis[U"MoveY"]
			.add(KeyW | KeyUp, -1)
			.add(KeyS | KeyDown, 1)
			.add({ PadState::LThumbY, -1 });

		auto actor = createActor().lock();
		actor->attachComponent<MoveComponent>();
		actor->attachComponent<CircleComponent>();
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
}
