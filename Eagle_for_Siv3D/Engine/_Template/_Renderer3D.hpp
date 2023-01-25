#pragma once

#include <Framework/Renderer3D.hpp>

class MyRenderer3D : public eagle::Renderer3D
{
public:

	MyRenderer3D();

	virtual ~MyRenderer3D();

private:

	virtual void start()override;

	virtual void draw()const override;

};
