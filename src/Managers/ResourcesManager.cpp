#include "ResourcesManager.h"

ResourcesManager* ResourcesManager::resources_manager_ = nullptr;

bool ResourcesManager::load_scene_total(std::string path, std::string file_type)
{
    std::cout << "Loading all files: " << file_type << ", path: " << path << std::endl;
    return true;
}

bool ResourcesManager::load_scene(std::string path, std::string file_type)
{
    std::cout << "Loading file: " << file_type << ", path: " << path << std::endl;
    return true;
}

bool ResourcesManager::load_scene(SceneType type)
{
    uint64_t time = SDL_GetTicks64();
    if (type == SceneType::MainMenu)
    {
        load_texture("resources\\2.png");
        load_texture("resources\\4.png");
        load_texture("resources\\3.png");
        load_atlas("resources\\6.png", glm::vec2{11, 5});
        load_atlas("resources\\7.png", glm::vec2{11, 5});
    }

    uint64_t end = SDL_GetTicks64();
    std::cout << "Load resources time: " << end - time << std::endl;

    return true;
}

void ResourcesManager::load_texture(const std::string& path)
{
    if (scene_texture_.find(path) != scene_texture_.end())
    {
        return;
    }
    auto t = std::make_unique<Texture>();
    t->load(path, renderer_->get_renderer());
    scene_texture_[path] = std::move(t);
}

void ResourcesManager::load_texture(const std::string& path, std::string name)
{
    auto t = std::make_unique<Texture>();
    t->load(path, renderer_->get_renderer());
    scene_texture_[name] = std::move(t);
}

Texture* ResourcesManager::get_texture(std::string name)
{
    if (scene_texture_.find(name) != scene_texture_.end())
    {
        return scene_texture_[name].get();
    }
    return nullptr;
}

void ResourcesManager::clear_scene_resources()
{
    scene_texture_.clear();
    scene_atlas_.clear();
    scene_font_.clear();
}

void ResourcesManager::load_atlas(const std::string path)
{
    load_texture(path);
    auto a = std::make_unique<Atlas>(get_texture(path));
    scene_atlas_[path] = std::move(a);
}

void ResourcesManager::load_atlas(const std::string path, std::string name)
{
    load_texture(path, name);
    auto a = std::make_unique<Atlas>(get_texture(name));
    scene_atlas_[name] = std::move(a);
}

void ResourcesManager::load_atlas(const std::string path, glm::vec2 cell)
{
    load_texture(path);
    auto a = std::make_unique<Atlas>(get_texture(path), cell);
    scene_atlas_[path] = std::move(a);
}

Atlas* ResourcesManager::get_atlas(std::string name)
{
    if (!scene_atlas_[name])
        return nullptr;
    return scene_atlas_[name].get();
}

void ResourcesManager::set_renderer(SDLRender* renderer)
{
    renderer_ = renderer;
}

uint16_t ResourcesManager::register_danmaku_texture(const std::string path)
{
    std::unique_ptr<Texture> t = std::make_unique<Texture>();
    t->load(path, renderer_->get_renderer());
    danmaku_texture_.push_back(std::move(t));
    return static_cast<uint16_t>(danmaku_texture_.size() - 1);
}

Texture* ResourcesManager::get_danmaku_texture(uint16_t id)
{
    return danmaku_texture_[id].get();
}