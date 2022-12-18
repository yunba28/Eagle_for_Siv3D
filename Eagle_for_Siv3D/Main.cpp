# include <Eagle.hpp> // OpenSiv3D v0.6.6

class CircleComponent : public eagle::DrawableComponent2D
{
public:

	CircleComponent(){}

	void draw()const override
	{
		Circle{ getTransform()->getWorldPos2D(),20 }.draw();
	}
};

class Scene01 : public eagle::DefaultWorld::BasicScene
{
public:

	Scene01(const InitData& _ini)
		: BasicScene(_ini)
	{
		{
			auto actor = createActor(U"Actor").lock();

			auto tf = actor->getTransform();
			tf->setPos(200, 200);

			auto circle = actor->attachComponent<CircleComponent>().lock();
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
	eagle::DefaultWorld world{};
	world.add<Scene01>(U"Sc");


	while (System::Update())
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
