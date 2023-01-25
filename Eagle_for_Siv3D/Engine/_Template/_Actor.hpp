#pragma once

#include <Framework/Actor.hpp>

class MyActor : public eagle::Actor
{
public:

	MyActor();

	virtual ~MyActor();

private:

	virtual void awake()override;

	virtual void dispose()override;

};
