#pragma once
class IWindows
{
public:
	IWindows() = default;
	virtual ~IWindows() = default;

	virtual void init() = 0;


protected:


};

