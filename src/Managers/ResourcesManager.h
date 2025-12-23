#pragma once
#include"includes.h"
#include"Texture.h"
#include<unordered_map>

class ResourcesManager
{
public:
	
	static ResourcesManager* getInstance() {
		if (!resources_manager) {
			resources_manager = new ResourcesManager;
		}

		return resources_manager;
	};

	~ResourcesManager();//析构后清除所有资源

	bool loadSceneTotal(std::string path, std::string file_type);
	//更具文件类型"json或者xml加载场景"
	bool loadScene(std::string path, std::string file_type);

	//清除资源 记得delete掉堆上的资源
	void clearSceneResources();
	void clearAllResources();


	ResourcesManager(const ResourcesManager& res) = delete;
	ResourcesManager& operator=(const ResourcesManager& res) = delete;
	
	void load_texture(std::string path,ImageData type);
	Texture* get_texture(std::string name);


private:
	ResourcesManager() = default;
	std::unordered_map<std::string, Texture> global_Texture;
	std::unordered_map<std::string, Font> global_Font;
	std::unordered_map<std::string, Atlas> global_Atlas;

	//使用智能指针进行管理 避免内存泄露与悬空指针
	std::unordered_map<std::string, Texture*> Scene_Texture;
	std::unordered_map<std::string, Font*> Scene_Font;
	std::unordered_map<std::string, Atlas*> Scene_Atlas;

	static ResourcesManager* resources_manager;
};

