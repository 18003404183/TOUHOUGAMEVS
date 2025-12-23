#include "ResourcesManager.h"

ResourcesManager* ResourcesManager::resources_manager = nullptr;

bool ResourcesManager::loadSceneTotal(std::string path, std::string file_type)
{
	std::cout << "解析全局文件" << file_type << "中" << ",在路径" << path << std::endl;

	return true;
}

bool ResourcesManager::loadScene(std::string path, std::string file_type)
{
	//解析文件
	std::cout << "解析文件" << file_type << "中" << ",在路径" << path << std::endl;

	//将加载资源至哈希表

	return true;
}

void ResourcesManager::load_texture(std::string path,ImageData type){
	Texture* t = new Texture;
	t->load(path,type);
	this->Scene_Texture[path] = t;
}

Texture* ResourcesManager::get_texture(std::string name) {
	return this->Scene_Texture[name];
}