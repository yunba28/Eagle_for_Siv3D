#pragma once

#include <Framework/Behavior.hpp>

class MyBehavior : public eagle::Behavior
{
public:

	MyBehavior();

	virtual ~MyBehavior();

private:

	virtual void start()override;

	virtual void update()override;

};
