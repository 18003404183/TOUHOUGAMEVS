#pragma once

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

enum class ImageData
{
    ImageE,
    ImageS
};

class SDLRender;

class Texture
{
public:
    Texture() : data(nullptr), w(0), h(0) {}
    ~Texture() { free_data(); }

    // non-copyable
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // movable
    Texture(Texture&& other) noexcept : data(other.data), w(other.w), h(other.h) { other.data = nullptr; other.w = other.h = 0; }

    Texture& operator=(Texture&& other) noexcept 
    { 
    if (this != &other) 
    { 
        free_data(); 
        data = other.data; 
        w = other.w; 
        h = other.h; 
        other.data = nullptr; 
        other.w = other.h = 0;
    } 
    return *this; 
    }

    // Load using SDL renderer. If renderer==nullptr the load fails for SDL textures.
    bool load(const std::string& path, ImageData type, SDL_Renderer* renderer = nullptr);

    SDL_Texture* native() const { return data; }
    bool is_valid() const { return data != nullptr; }
    int width() const { return w; }
    int height() const { return h; }

    // Backwards-compatible helper: template to mimic previous get_data_as usage
    template<typename T>
    T get_data_as(ImageData id)
    {
        if constexpr (std::is_same_v<T, SDL_Texture*>) {
            if (id == ImageData::ImageS) return data;
            return nullptr;
        }
        return nullptr;
    }

protected:
    void free_data();

private:
    SDL_Texture* data;
    int w, h;
};