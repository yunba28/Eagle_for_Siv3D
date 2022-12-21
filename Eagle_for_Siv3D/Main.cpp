# include <Eagle.hpp> // OpenSiv3D v0.6.6

#include <Components/Colliders2D.hpp>
#include <Components/Renderer.hpp>
#include <Components/Animation.hpp>
#include <Utility/Loader/Load.hpp>

class MoveComponent : public eagle::Component
{
public:

private:

	void start()override
	{
		mCollider = getActor()->getComponent<eagle::PolygonCollider2D>();
		mAnimController = getActor()->getComponent<eagle::AnimationController2D>();
		joinFixedSystem();
	}

	void update()override
	{
		mMoveScale = KeyRight.pressed() - KeyLeft.pressed();

		if (mIsGround and (not mIsJump))
		{
			mIsJump = KeyUp.down();
		}

		auto anim = mAnimController.lock();

		if (not anim)
			return;

		if (mMoveScale != 0.0)
		{
			mLookRight = mMoveScale > 0.0;
			if (mLookRight)
			{
				anim->setInt(U"MoveState", 2);
			}
			else
			{
				anim->setInt(U"MoveState", -2);
			}
		}
		else
		{
			if (mLookRight)
			{
				anim->setInt(U"MoveState", 1);
			}
			else
			{
				anim->setInt(U"MoveState", -1);
			}
		}

		Print << U"Ground:{}"_fmt(mIsGround);
		Print << U"Jump:{}"_fmt(mIsJump);
	}

	void fixedUpdate()override
	{
		auto collider = mCollider.lock();

		if (mMoveScale != 0.0)
		{
			Vec2 force = Vec2{ mMoveScale * 2500, 0 };
			collider->addForce(force);
		}

		if (mIsJump)
		{
			auto mass = collider->getMass();
			collider->addImpulse(0, -750 * mass);
			collider->translate(0, -5);
			mIsGround = false;
			mIsJump = false;
		}
	}

	void onCollisionEnter(const eagle::Collision& _col)override
	{
		auto actor = _col.opponent.lock();
		if (actor->compareTag(U"Ground"))
		{
			mIsGround = true;
		}
	}

	void onCollisionStay(const eagle::Collision& _col)override
	{
		auto actor = _col.opponent.lock();
		if (actor->compareTag(U"Ground"))
		{
			mIsGround = true;
		}
	}

	void onCollisionExit(const eagle::Collision& _col)override
	{
		auto actor = _col.opponent.lock();
		if (actor->compareTag(U"Ground"))
		{
			mIsGround = false;
		}
	}

private:

	eagle::WeakObject<eagle::PolygonCollider2D> mCollider;

	eagle::WeakObject<eagle::AnimationController2D> mAnimController;

	double mMoveScale{ 0 };

	bool mLookRight{ true };

	bool mIsGround{ false };

	bool mIsJump{ false };

};

// シーンの作成
class TestScene : public eagle::DefaultWorld::BasicScene
{
public:
	TestScene(const InitData& _ini) : BasicScene(_ini)
	{
		// プレイヤー
		{
			auto actor = createActor(U"Player").lock();
			actor->addTag(U"Player");
			Polygon body = RoundRect{ -32,-128,64,128,32 }.asPolygon(4);
			auto collider = actor->attachComponent<eagle::PolygonCollider2D>().lock();
			auto animationController = actor->attachComponent<eagle::AnimationController2D>().lock();
			actor->attachComponent<MoveComponent>();
			eagle::Load(U"player.col", *collider);
			eagle::Load(U"Player.anim", *animationController);
		}

		// 床
		{
			auto actor = createActor(U"Ground").lock();
			actor->addTag(U"Ground");
			auto collider = actor->attachComponent<eagle::LineCollider2D>().lock();
			collider->setupLine(Line{ Vec2{-300,0},Vec2{300,0} }, OneSided::No, P2Material{ .density = 1.0,.friction = 0.5 });
			collider->setPos(400, 500, true);
			collider->setBodyType(P2BodyType::Static);
		}

		// 壁
		{
			auto actor = createActor(U"Wall").lock();
			actor->addTag(U"Wall");
			auto collider = actor->attachComponent<eagle::LineCollider2D>().lock();
			collider->setupLine(Line{ Vec2{0,0},Vec2{0,-500} }, OneSided::No, P2Material{ .density = 0.2,.friction = 0.0 });
			collider->setPos(100, 500, true);
			collider->setBodyType(P2BodyType::Static);
		}

		// 壁
		{
			auto actor = createActor(U"Wall").lock();
			actor->addTag(U"Wall");
			auto collider = actor->attachComponent<eagle::LineCollider2D>().lock();
			collider->setupLine(Line{ Vec2{0,0},Vec2{0,-500} }, OneSided::No, P2Material{ .density = 0.2,.friction = 0.0 });
			collider->setPos(700, 500, true);
			collider->setBodyType(P2BodyType::Static);
		}

	}

	void update()override
	{
		
	}

	void draw()const override
	{

	}


};

void Main()
{
	// リソースの事前登録
	eagle::Resource::LoadTextures(U"texture");

	// シーンを管理するマネジメントクラスを生成
	eagle::DefaultWorld world{};
	// TestSceneをMySceneという名前でworldに登録
	world.add<TestScene>(U"MyScene");

	while (s3d::System::Update())
	{
		ClearPrint();

		if (not world.update())
			break;
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要です。
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/tutorial/
//
// Tutorial
// https://siv3d.github.io/tutorial/tutorial/
//
// Siv3D コミュニティへの参加（Discord などで気軽に質問や交流, 最新情報の入手ができます）
// https://siv3d.github.io/ja-jp/community/community/
//
// Siv3D User Community
// https://siv3d.github.io/community/community/
//
// 新機能の提案やバグの報告 | Feedback
// https://siv3d.github.io/ja-jp/develop/report/
//
// Sponsoring Siv3D
// https://github.com/sponsors/Reputeless
//
