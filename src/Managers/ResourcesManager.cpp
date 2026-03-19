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
		this->load_texture("resources\\4.png");
		this->load_texture("resources\\3.png");
		this->load_atlas("resources\\6.png",glm::vec2{11,5});
		this->load_atlas("resources\\7.png",glm::vec2{11,5});
	}

	uint64_t end = SDL_GetTicks64();
	std::cout<<"加载资源时间"<<end-time<<std::endl;

	return true; 
}

void ResourcesManager::load_texture(const std::string& path){
	//将裸指针实现改为智能指针
	if(Scene_Texture.find(path) != Scene_Texture.end()){
		return;
	}
	auto t = std::make_unique<Texture>();
	t->load(path,this->renderer->get_renderer());
	this->Scene_Texture[path] = std::move(t);
}

void ResourcesManager::load_texture(const std::string &path, std::string name)
{
	auto t = std::make_unique<Texture>();
	t->load(path,this->renderer->get_renderer());
	this->Scene_Texture[name] = std::move(t);
}

Texture* ResourcesManager::get_texture(std::string name) {
	if(Scene_Texture.find(name) != Scene_Texture.end()){
		return this->Scene_Texture[name].get();
	}
	return nullptr;
}

void ResourcesManager::clear_scene_resources(){
	Scene_Texture.clear();
	Scene_Atlas.clear();
	Scene_Font.clear();
	
}

void ResourcesManager::load_atlas(const std::string path)
{
	this->load_texture(path);
	auto a = std::make_unique<Atlas>(this->get_texture(path));
	this->Scene_Atlas[path] = std::move(a);
}

void ResourcesManager::load_atlas(const std::string path, std::string name)
{
	this->load_texture(path,name);
	auto a = std::make_unique<Atlas>(this->get_texture(name));
	this->Scene_Atlas[name] = std::move(a);

}

void ResourcesManager::load_atlas(const std::string path,glm::vec2 cell)
{
	this->load_texture(path);
	auto a = std::make_unique<Atlas>(this->get_texture(path),cell);
	this->Scene_Atlas[path] = std::move(a);
}
Atlas *ResourcesManager::get_atlas(std::string name)
{
	if(!this->Scene_Atlas[name])
    	return nullptr;
	return Scene_Atlas[name].get();
}


void ResourcesManager::set_renderer(SDLRender* renderer){
 	this->renderer = renderer;
}

uint16_t ResourcesManager::register_danmaku_texture(const std::string path)
{	
	std::unique_ptr<Texture> t = std::make_unique<Texture>();
	t.get()->load(path,this->renderer->get_renderer());
	this->danmaku_texture.push_back(std::move(t));
    return static_cast<uint16_t>(danmaku_texture.size() - 1);
}

Texture* ResourcesManager::get_danmaku_texture(uint16_t id)
{
    return this->danmaku_texture[id].get();
}
