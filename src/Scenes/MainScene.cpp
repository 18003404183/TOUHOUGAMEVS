#include "MainScene.h"
#include "SceneManager.h"
#include "EasyxRender.h"
#include "ResourcesManager.h"
#include "clock.h"

MainScene::MainScene()
{
	IInput::get();
	std::cout << "MainScene Constructor" << std::endl;

}

MainScene::~MainScene()
{
	std::cout << "MainScene Destructor" << std::endl;
}


void MainScene::on_enter()
{
	ResourcesManager::getInstance()->loadScene(SceneType::MainMenu);
	
	std::cout << "Entering MainScene" << std::endl;
	EnemyBuilder enemybuilder;
	enemybuilder.create_enemy(EnemyType::small);
	Enemy* a = enemybuilder.create();
	this->renderables.push_back(a);
	this->updateables.push_back(a);

	enemybuilder.create_enemy(EnemyType::common);
	Enemy* b = enemybuilder.create();
	this->renderables.push_back(b);
	this->updateables.push_back(b);

	enemybuilder.create_enemy(EnemyType::large);
	Enemy* c = enemybuilder.create();
	this->renderables.push_back(c);
	this->updateables.push_back(c);
	//this->clock();
	//调用this后relese模式下程序暂停 debug没有问题

	this->clock.setCallback([this](){

		std::cout<<this<<std::endl;//没问题
		std::cout<<"daugsjdha"<<"this:"<<this<<std::endl;//没问题
		std::cout<<"踹刷u读书u和古代u上高低"<<"this:"<<this<<std::endl;//没问题
		//std::cout<<"踹刷u读书u和古代u上高低"<<this<<std::endl;//程序强制暂停
	});
	Texture* v = ResourcesManager::getInstance()->get_texture("resources\\2.png");
	glm::vec2 pos;
	glm::vec2 scale;
	pos.x = 205;
	pos.y = 205;
	scale.x = 1;
	scale.y = 1;

	IRenderable* image = new Image(v,pos,scale,0,255);
	this->renderables.push_back(image);
}

void MainScene::on_exit()
{
	std::cout << "Exiting MainScene" << std::endl;
}

void MainScene::on_update(float deltatime)
{ 
	for (IUpdateable* a : this->updateables) {
		a->update(deltatime);
	}
	this->clock.update(deltatime);
	IInput::update();

	std::cout << "Updating MainScene: " << deltatime << " seconds elapsed." << std::endl;
}

void MainScene::on_render(SDLRender& renderer)
{
	
	// Texture* a = ResourcesManager::getInstance()->get_texture("resources\\2.png");
	// renderer->draw_texture(a, 0,0);

	for (IRenderable* a : this->renderables) {
		a->render(renderer);
	}
	//render->draw_text("MainScene",10,10);
	std::cout << "Rendering MainScene" << std::endl;
}

void MainScene::on_input()
{
	if(IInput::get_key(KeyType::ENTER)->get_keydown())
		SceneManager::getInstance()->switchScene(SceneType::EndMenu);
	
}
