#include"SDLRender.h"
#include "../Resources/Texture.h"
#include <iostream>

void SDLRender::init(SDL_Window* window){
    if(window == nullptr) return;
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//开启垂直同步来保持平滑
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

void SDLRender::draw_texture(
        Texture* texture, 
        const glm::vec2& pos, 
        const glm::vec2& scale,
        const glm::vec2& wh,
        double rotation, 
        unsigned char alpha){
    //pos代表图像的中心点

    float finalW = wh.x * scale.x;
    float finalH = wh.y * scale.y;

    float dstRectx = pos.x - (finalW / 2.0f); 
    float dstRecty = pos.y - (finalH / 2.0f);

    SDL_FRect dst;
    dst.x = dstRectx;
    dst.y = dstRecty;
    dst.w = finalW;
    dst.h = finalH;
    //{image.get_pos().x,image.get_pos().y,(image.get_wh().x*image.get_scale().x),(image.get_wh().y*image.get_scale().y)};

    // 中心决定了如何旋转 一般旋转中心为图片中心
    // sdl的 center是按dst.x 与 dst.y 为原点的
    SDL_FPoint center;
    //center.x = pos.x;
    //center.y = pos.y;
    center.x = finalW/2.0f;
    center.y = finalH/2.0f;

    SDL_RenderCopyExF(
        this->renderer,
        texture->get_data(),
        NULL,
        &dst,
        rotation,
        &center,
        SDL_RendererFlip::SDL_FLIP_NONE
    );

}

void SDLRender::draw_texture(Texture *texture, const glm::vec2 &pos, const glm::vec2 &scale, const glm::vec2 &wh, const SDL_Rect& cell, double rotation, unsigned char alpha)
{
    if(!texture){
        std::cerr << "绘制内容为空" << std::endl;
        return;
    }
    float finalW = wh.x * scale.x;
    float finalH = wh.y * scale.y;

    float dstRectx = pos.x - (finalW / 2.0f); 
    float dstRecty = pos.y - (finalH / 2.0f);

    SDL_FRect dst;
    dst.x = dstRectx;
    dst.y = dstRecty;
    dst.w = finalW;
    dst.h = finalH;
    //{image.get_pos().x,image.get_pos().y,(image.get_wh().x*image.get_scale().x),(image.get_wh().y*image.get_scale().y)};
    SDL_FPoint center;
    //center.x = pos.x;
    //center.y = pos.y;
    center.x = finalW/2.0f;
    center.y = finalH/2.0f;

    SDL_RenderCopyExF(
        this->renderer,
        texture->get_data(),
        &cell,
        &dst,
        rotation,
        &center,
        SDL_RendererFlip::SDL_FLIP_NONE
    );
}

void SDLRender::draw_text(std::string text, int x, int y){
    // Minimal placeholder: text rendering not implemented (requires SDL_ttf)
    (void)text; (void)x; (void)y;
    std::cerr << "[SDLRender] draw_text not implemented (requires SDL_ttf)" << std::endl;
}

void SDLRender::draw_circle_outline(const glm::vec2 pos, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(this->renderer,r,g,b,a);

    const int NUM_SEGMENTS = 32;
    SDL_FPoint points[NUM_SEGMENTS+1];

    for(int i = 0; i<NUM_SEGMENTS+1;i++){
        float theta = ((i/(float)NUM_SEGMENTS)*2*3.1415926f);

        points[i].x = pos.x + (cos(theta)*radius);
        points[i].y = pos.y + (sin(theta)*radius);
    }

    SDL_RenderDrawLinesF(this->renderer,points,NUM_SEGMENTS+1);

}
