#include "ResourcesManager.h"

ResourcesManager* ResourcesManager::resources_manager = nullptr;

bool ResourcesManager::loadSceneTotal(std::string path, std::string file_type)
{
	std::cout << "����ȫ���ļ�" << file_type << "��" << ",��·��" << path << std::endl;

	return true;
}

bool ResourcesManager::loadScene(std::string path, std::string file_type)
{
	//�����ļ�
	std::cout << "�����ļ�" << file_type << "��" << ",��·��" << path << std::endl;
	
	//��������Դ����ϣ��

	return true;
}

bool ResourcesManager::loadScene(SceneType type){

	if(type == SceneType::MainMenu){
		ResourcesManager::getInstance()->load_texture("resources\\2.png",ImageData::ImageS,this->renderer);
	}

	return true;
}

void ResourcesManager::load_texture(const std::string& path, ImageData type, SDLRender* renderer){
	Texture* t = new Texture;
	t->load(path,renderer->get_renderer());
	this->Scene_Texture[path] = t;
}

Texture* ResourcesManager::get_texture(std::string name) {
	return this->Scene_Texture[name];
}

void ResourcesManager::set_renderer(SDLRender* renderer){
	this->renderer = renderer;
}