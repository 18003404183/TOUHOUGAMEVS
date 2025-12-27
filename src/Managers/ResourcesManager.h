#pragma once
#include"includes.h"
#include"Texture.h"
#include<unordered_map>
#include"IRender.h"
#include"SceneType.h"
#include"SDLRender.h"

class ResourcesManager
{
public:
	
	static ResourcesManager* getInstance() {
		if (!resources_manager) {
			resources_manager = new ResourcesManager;
		}

		return resources_manager;
	};

	~ResourcesManager();//���������������Դ

	bool loadSceneTotal(std::string path, std::string file_type);
	//�����ļ�����"json����xml���س���"
	bool loadScene(std::string path, std::string file_type);

	bool loadScene(SceneType type);

	//�����Դ �ǵ�delete�����ϵ���Դ
	void clearSceneResources();
	void clearAllResources();


	ResourcesManager(const ResourcesManager& res) = delete;
	ResourcesManager& operator=(const ResourcesManager& res) = delete;
	
	void load_texture(const std::string& path, ImageData type, SDLRender* renderer = nullptr);
	Texture* get_texture(std::string name);

	void set_renderer(SDLRender* renderer);

private:
	ResourcesManager() = default;
	std::unordered_map<std::string, Texture> global_Texture;
	std::unordered_map<std::string, Font> global_Font;
	std::unordered_map<std::string, Atlas> global_Atlas;

	//ʹ������ָ����й��� �����ڴ�й¶������ָ��
	std::unordered_map<std::string, Texture*> Scene_Texture;
	std::unordered_map<std::string, Font*> Scene_Font;
	std::unordered_map<std::string, Atlas*> Scene_Atlas;

	static ResourcesManager* resources_manager;

	SDLRender* renderer = nullptr;
	
};

