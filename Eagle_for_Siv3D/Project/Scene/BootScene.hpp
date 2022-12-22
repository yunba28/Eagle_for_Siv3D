#pragma once

#include "../ProjectCommons.hpp"

class BootScene final : public eagle::DefaultWorld::BasicScene
{
public:

	BootScene(const InitData& _ini);

	~BootScene();

	void update()override;

	void draw()const override;

private:

};
