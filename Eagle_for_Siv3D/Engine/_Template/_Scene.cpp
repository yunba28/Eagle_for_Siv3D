#include "_Scene.hpp"

MyScene::MyScene(const InitData& _init)
	: Super(_init)
{
}

MyScene::~MyScene()
{
}

void MyScene::awake()
{
}

void MyScene::dispose()
{
}

void MyScene::update()
{
	/// @brief この処理を書かないとActor・Componentの更新処理が呼ばれません
	Super::update();

}

void MyScene::draw() const
{
	/// @brief この処理を書かないとRenderer2D・Renderer3Dの描画処理が呼ばれません
	Super::draw();

}
