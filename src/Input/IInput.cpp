#include "IInput.h"

IInput* IInput::input = nullptr;

IInput::IInput() {
	for (int i = 0; i < (int)KeyType::_count; i++) {
		keys[i] = new Key((KeyType)i);
	}
}
IInput* IInput::get() {
	if (input == nullptr)input = new IInput();
	return input;
}
bool IInput::update() {
	bool upd = 0;
	get();
	for (int i = 0; i < (int)KeyType::_count; i++) {
		upd = upd || input->keys[i]->update();
	}
	return upd;
}
Key* IInput::get_key(KeyType k) {
	return get()->keys[(int)k];
}