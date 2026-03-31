#pragma once
#include"includes.h"
#include"Texture.h"
#include"Atlas.h"
#include"Font.h"
#include<unordered_map>
#include"IRender.h"
#include"SceneType.h"
#include"SDLRender.h"
#include"Danmaku.h"

class ResourcesManager
{
public:
    static ResourcesManager* get_instance()
    {
        if (!resources_manager_)
        {
            resources_manager_ = new ResourcesManager;
        }
        return resources_manager_;
    }

    ~ResourcesManager() = default;

    bool load_scene_total(std::string path, std::string file_type);
    bool load_scene(std::string path, std::string file_type);
    bool load_scene(SceneType type);

    void clear_scene_resources();
    void clear_all_resources();


	ResourcesManager(const ResourcesManager& res) = delete;
	ResourcesManager& operator=(const ResourcesManager& res) = delete;
	
	void load_texture(const std::string& path);
	void load_texture(const std::string& path, std::string name);
	Texture* get_texture(std::string name);

	void load_atlas(const std::string path);
	void load_atlas(const std::string path, std::string name);
	void load_atlas(const std::string path, glm::vec2 cell);
	Atlas* get_atlas(std::string name);

	void set_renderer(SDLRender* renderer);

	uint16_t register_danmaku_texture(const std::string path);
	Texture* get_danmaku_texture(uint16_t id);

private:
    ResourcesManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Texture>> global_texture_;
    std::unordered_map<std::string, std::unique_ptr<Font>> global_font_;
    std::unordered_map<std::string, std::unique_ptr<Atlas>> global_atlas_;

    std::unordered_map<std::string, std::unique_ptr<Texture>> scene_texture_;
    std::unordered_map<std::string, std::unique_ptr<Font>> scene_font_;
    std::unordered_map<std::string, std::unique_ptr<Atlas>> scene_atlas_;

    std::vector<std::unique_ptr<Texture>> danmaku_texture_;

    static ResourcesManager* resources_manager_;

    SDLRender* renderer_ = nullptr;
	
};