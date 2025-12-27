#pragma once
#include "IWindows.h"
#include <easyx.h>

class EasyxWindow :
    public IWindows
{
public:
    EasyxWindow();
    ~EasyxWindow();
	bool init(int w,int h) override;
};

