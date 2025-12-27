#include"SDLRender.h"
#include "../Resources/Texture.h"
#include <iostream>

void SDLRender::init(SDL_Window* window){
    if(window == nullptr) return;
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void SDLRender::clear(){
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // 设置黑色
    SDL_RenderClear(this->renderer); // 清理
}

void SDLRender::present() {
    SDL_RenderPresent(this->renderer); // 显示
}


// void SDLRender::draw_texture(Texture* image, int x, int y){
// }

// void SDLRender::draw_text(std::string text, int x, int y){
// }

void SDLRender::draw_texture(Texture* image, int x, int y){
    if (!image || !image->is_valid()) return;
    SDL_Texture* tex = image->native();
    if (!tex || !this->renderer) return;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = image->width();
    dst.h = image->height();

    SDL_RenderCopy(this->renderer, tex, nullptr, &dst);
}

void SDLRender::draw_text(std::string text, int x, int y){
    // Minimal placeholder: text rendering not implemented (requires SDL_ttf)
    (void)text; (void)x; (void)y;
    std::cerr << "[SDLRender] draw_text not implemented (requires SDL_ttf)" << std::endl;
}
