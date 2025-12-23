#pragma once

#include "KeyType.h"
#include "includes.h"

class Key
{
public:
	Key(KeyType key);
	const KeyType key_;
	bool update();
	bool get_state();
	bool get_keydown();
	bool get_keyup();
private:
	bool state;
	bool state_prev;
};



