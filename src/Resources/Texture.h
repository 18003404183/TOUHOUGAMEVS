#pragma once

#include <easyx.h>
#include <variant>
#include <string>
#include <iostream>

// ==========================================
// 前置声明 (核心技巧)
// 告诉编译器这俩名字是结构体，先别报错，具体的定义以后有了头文件再说
// ==========================================
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
    // 默认构造：初始化为空
    Texture() : Data((IMAGE*)nullptr) {}

    // 析构：释放资源
    ~Texture() {
        free_data();
    }

    // 兼容旧接口
    Texture(IMAGE* ImageData) : Data(ImageData) {}
    
    // 预留接口：虽然现在没有SDL库，但可以使用指针类型
    Texture(SDL_Texture* SdlData) : Data(SdlData) {}

    // ==========================================
    // 新增：加载功能 (带预留)
    // ==========================================
    bool load(const std::string& path, ImageData type, SDL_Renderer* renderer = nullptr)
    {
        // 1. 先清理旧数据
        free_data();

        if (type == ImageData::ImageE) {
            // --- EasyX 实现 ---
            IMAGE* img = new IMAGE();
            
            // 注意：如果项目是 Unicode 字符集，这里可能需要转换路径编码
            // 这里假设是 多字节字符集 或者你可以自行处理 string 到 LPCTSTR 的转换
            loadimage(img, path.c_str());

            if (img->getwidth() == 0) { // 简单的加载失败检查
                delete img;
                return false;
            }
            Data = img;
            return true;
        }
        else if (type == ImageData::ImageS) {
            // --- SDL 预留位 ---
            // 等你以后有了 SDL 环境，把这段代码取消注释即可
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
    // 获取数据 (使用 std::get_if 安全获取)
    // ==========================================
    template<typename T>
    T get_data_as(ImageData ID)
    {
        if (ID == ImageData::ImageE) {
            // 尝试获取 IMAGE*
            if (auto val = std::get_if<IMAGE*>(&Data)) {
                if constexpr (std::is_same_v<T, IMAGE*>) {
                    return *val;
                }
            }
        }
        else if (ID == ImageData::ImageS) {
            // 尝试获取 SDL_Texture*
            // 这里虽然没有 SDL 库，但 T == SDL_Texture* 的判断是合法的
            if (auto val = std::get_if<SDL_Texture*>(&Data)) {
                if constexpr (std::is_same_v<T, SDL_Texture*>) {
                    return *val;
                }
            }
        }
        return nullptr;
    }

    // ==========================================
    // 有效性检查
    // ==========================================
    bool is_valid() const
    {
        return std::visit([](auto&& arg) -> bool {
            // 无论是 IMAGE* 还是 SDL_Texture*，只要不是空指针就有效
            return arg != nullptr;
        }, Data);
    }

protected:
    
    void free_data() {
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            
            if constexpr (std::is_same_v<T, IMAGE*>) {
                // EasyX 销毁
                if (arg) delete arg;
            }
            else if constexpr (std::is_same_v<T, SDL_Texture*>) {
                // SDL 销毁 (预留)
                // 以后有了 SDL 库，这里写： if (arg) SDL_DestroyTexture(arg);
            }
        }, Data);

        // 重置状态
        Data = (IMAGE*)nullptr;
    }

    // 核心存储
    std::variant<IMAGE*, SDL_Texture*> Data;
};