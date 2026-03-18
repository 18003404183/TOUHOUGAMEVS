#include "Key.h"
#include <SDL.h>

Key::Key(KeyType key):key_(key){
    state = false;
    state_prev = false;
}

bool Key::update() {
    state_prev = state;
    
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    
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