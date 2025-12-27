#pragma once
class IWindows
{
public:
	IWindows() = default;
	virtual ~IWindows() = default;

	virtual bool init(int w,int h) = 0;

protected:

};

