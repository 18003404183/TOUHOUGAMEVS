#pragma once
class IUpdateable
{
public:
	virtual void update(float deltaTime) = 0;
	virtual ~IUpdateable() = default;
};

