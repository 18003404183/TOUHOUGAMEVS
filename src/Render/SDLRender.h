#pragma once

#include "IRender.h"
#include <SDL.h>
#include "includes.h"

class SDLRender
{
public:
    SDLRender() = default;

    virtual void init(SDL_Window* window = nullptr);

    virtual void clear();

    virtual void draw_texture(Texture* image, int x, int y);

    virtual void draw_texture(
        Texture* texture,
        const glm::vec2& pos,
        const glm::vec2& scale,
        const glm::vec2& size,
        double rotation,
        unsigned char alpha);

    virtual void draw_texture(
        Texture* texture,
        const glm::vec2& pos,
        const glm::vec2& scale,
        const glm::vec2& size,
        const SDL_Rect& cell,
        double rotation,
        unsigned char alpha);

    virtual void draw_text(const std::string& text, int x, int y);

    virtual void draw_circle_outline(
        const glm::vec2& pos,
        float radius,
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a = 255);

    SDL_Renderer* get_renderer() const
    {
        return renderer_;
    }

    void present();

private:
    SDL_Renderer* renderer_ = nullptr;
};