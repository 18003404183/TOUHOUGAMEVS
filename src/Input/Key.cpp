#include "Key.h"
#include <SDL.h>

Key::Key(KeyType key):key_(key){
    state = false;
    state_prev = false;
}

bool Key::update() {
    state_prev = state;
    
    // 获取 SDL 内部维护的全局键盘状态数组
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
    // 直接通过数组下标极速寻址，判断当前键是否被按下
    state = currentKeyStates[Keys[(int)key_]];
    
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