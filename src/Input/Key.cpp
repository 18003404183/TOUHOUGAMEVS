#include "Key.h"


Key::Key(KeyType key):key_(key){
	state = 0;
	state_prev = 0;
}
bool Key::update() {
	state_prev = state;
	state = (GetAsyncKeyState(Keys[(int)key_]) & 0x8000) != 0;
	return state_prev != state;
}
bool Key::get_state() {
	return state;
}
bool Key::get_keydown() {
	return (!state_prev) && state;
}
bool Key::get_keyup() {
	return state_prev && (!state);
}