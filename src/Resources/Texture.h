#pragma once

#include <easyx.h>
#include <variant>
#include <string>
#include <iostream>
#include <SDL.h>

struct SDL_Texture;
struct SDL_Renderer; 

enum class ImageData
{
    ImageE,
    ImageS
};

class Texture 
{
public:

    Texture() : Data((IMAGE*)nullptr) {}
    ~Texture() {
        free_data();
    }

    Texture(IMAGE* ImageData) : Data(ImageData) {}
    
    Texture(SDL_Texture* SdlData) : Data(SdlData) {}

    bool load(const std::string& path, ImageData type, SDL_Renderer* renderer = nullptr)
    {
        free_data();

        if (type == ImageData::ImageE) {
            // --- EasyX ʵ�� ---
            IMAGE* img = new IMAGE();
            
            loadimage(img, path.c_str());

            if (img->getwidth() == 0) {
                delete img;
                return false;
            }
            Data = img;
            return true;
        }
        else if (type == ImageData::ImageS) {
            // --- SDL Ԥ��λ ---
            // �����Ժ����� SDL ����������δ���ȡ��ע�ͼ���
            /*
            if (!renderer) return false;
            SDL_Surface* surf = IMG_Load(path.c_str());
            if (!surf) return false;
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);
            if (tex) {
                Data = tex;
                return true;
            }
            */
            std::cerr << "[Texture] SDL implementation not yet available." << std::endl;
            return false;
        }
        return false;
    }

    // ==========================================
    // ��ȡ���� (ʹ�� std::get_if ��ȫ��ȡ)
    // ==========================================
    template<typename T>
    T get_data_as(ImageData ID)
    {
        if (ID == ImageData::ImageE) {
            if (auto val = std::get_if<IMAGE*>(&Data)) {
                if constexpr (std::is_same_v<T, IMAGE*>) {
                    return *val;
                }
            }
        }
        else if (ID == ImageData::ImageS) {
            if (auto val = std::get_if<SDL_Texture*>(&Data)) {
                if constexpr (std::is_same_v<T, SDL_Texture*>) {
                    return *val;
                }
            }
        }
        return nullptr;
    }

    // ==========================================
    // ��Ч�Լ��
    // ==========================================
    bool is_valid() const
    {
        return std::visit([](auto&& arg) -> bool {
            // ������ IMAGE* ���� SDL_Texture*��ֻҪ���ǿ�ָ�����Ч
            return arg != nullptr;
        }, Data);
    }

protected:
    
    void free_data() {
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            
            if constexpr (std::is_same_v<T, IMAGE*>) {
                if (arg) delete arg;
            }
            else if constexpr (std::is_same_v<T, SDL_Texture*>) {
            }
        }, Data);

        Data = (IMAGE*)nullptr;
    }
    std::variant<IMAGE*, SDL_Texture*> Data;
};