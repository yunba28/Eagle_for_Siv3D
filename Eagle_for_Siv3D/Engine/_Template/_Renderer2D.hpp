#pragma once

#include <Framework/Renderer2D.hpp>

class MyRenderer2D : public eagle::Renderer2D
{
public:

	MyRenderer2D();

	virtual ~MyRenderer2D();

private:

	virtual void start()override;

	virtual void draw()const override;

};
