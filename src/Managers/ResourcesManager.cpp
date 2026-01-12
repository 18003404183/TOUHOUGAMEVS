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
	uint64_t time = SDL_GetTicks64();
	if(type == SceneType::MainMenu){
		this->load_texture("resources\\2.png");
		this->load_atlas("resources\\6.png",glm::vec2{11,5});
	}

	uint64_t end = SDL_GetTicks64();

	std::cout<<"加载资源时间"<<end-time<<std::endl;

	return true;
}

void ResourcesManager::load_texture(const std::string& path){
	Texture* t = new Texture;
	t->load(path,this->renderer->get_renderer());
	this->Scene_Texture[path] = t;
}

void ResourcesManager::load_texture(const std::string &path, std::string name)
{
	Texture* t = new Texture;
	t->load(path,this->renderer->get_renderer());
	this->Scene_Texture[name] = t;
}

Texture* ResourcesManager::get_texture(std::string name) {
	return this->Scene_Texture[name];
}

void ResourcesManager::load_atlas(const std::string path)
{
	Texture* t = new Texture;
	t->load(path,this->renderer->get_renderer());
	Atlas* a = new Atlas(t);
	this->Scene_Atlas[path] = a;
}

void ResourcesManager::load_atlas(const std::string path, std::string name)
{
	Texture* t = new Texture;
	t->load(path,this->renderer->get_renderer());
	Atlas* a = new Atlas(t);
	this->Scene_Atlas[name] = a;
}

void ResourcesManager::load_atlas(const std::string path,glm::vec2 cell)
{
	Texture* t = new Texture;
	t->load(path,this->renderer->get_renderer());
	Atlas* a = new Atlas(t,cell);
	this->Scene_Atlas[path] = a;
}

Atlas *ResourcesManager::get_atlas(std::string name)
{
	if(!this->Scene_Atlas[name])
    	return nullptr;
	return Scene_Atlas[name];
}


void ResourcesManager::set_renderer(SDLRender* renderer){
 	this->renderer = renderer;
}