#include"SDLRender.h"

void SDLRender::init(SDL_Window* window){
    if(window == nullptr) return;
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void SDLRender::clear(){


}

Texture* SDLRender::load_texture(const std::string path){

    return nullptr;
}

void SDLRender::draw_texture(Texture* image, int x, int y){


}

void SDLRender::draw_text(std::string text, int x, int y){


}
