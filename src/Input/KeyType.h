#pragma once
#include "includes.h"


enum class KeyType
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	Z,
	X,
	C,
	SHIFT,
	CTRL,
	ENTER,
	_count
};

const int Keys[] = { VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 'Z', 'X', 'C', VK_SHIFT, VK_CONTROL, VK_RETURN };

