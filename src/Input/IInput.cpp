#include "IInput.h"

IInput* IInput::input = nullptr;

IInput::IInput() {
    for (int i = 0; i < (int)KeyType::_count; i++) {
        keys[i] = new Key((KeyType)i);
    }
}

IInput* IInput::get() {
    if (input == nullptr) input = new IInput();
    return input;
}

bool IInput::update() {
    bool upd = false; // 严谨的初始化
    get();
    
    for (int i = 0; i < (int)KeyType::_count; i++) {

        bool key_changed = input->keys[i]->update();
        upd = upd || key_changed;
    }
    return upd;
}

Key* IInput::get_key(KeyType k) {
    return get()->keys[(int)k];
}