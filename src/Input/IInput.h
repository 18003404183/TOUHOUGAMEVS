#pragma once
#include "Key.h"

class IInput
{
public:
	static IInput* get();
	static bool update();
	static Key* get_key(KeyType k);
private:
	IInput();
	Key* keys[(const int)(KeyType::_count)];
	static IInput* input;
};

