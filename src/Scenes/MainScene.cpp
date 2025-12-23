#include "MainScene.h"
#include "SceneManager.h"
#include "EasyxRender.h"
#include "ResourcesManager.h"

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
	std::cout << "Entering MainScene" << std::endl;
	EnemyBuilder enemybuilder;
	enemybuilder.create_enemy(EnemyType::small);

	this->renderables.push_back(enemybuilder.create());
	this->updateables.push_back(enemybuilder.create());

	enemybuilder.create_enemy(EnemyType::common);
	this->renderables.push_back(enemybuilder.create());
	this->updateables.push_back(enemybuilder.create());

	enemybuilder.create_enemy(EnemyType::large);
	this->renderables.push_back(enemybuilder.create());
	this->updateables.push_back(enemybuilder.create());
	
	ResourcesManager::getInstance()->load_texture("resources\\2.png",ImageData::ImageE);
	

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
	IInput::update();
	std::cout << "Updating MainScene: " << deltatime << " seconds elapsed." << std::endl;
}

void MainScene::on_render()
{
	
	Texture* a = ResourcesManager::getInstance()->get_texture("resources\\2.png");
	this->render->draw_texture(a, 0,0);

	for (IRenderable* a : this->renderables) {
		a->render();
	}
	render->draw_text("MainScene",10,10);
	std::cout << "Rendering MainScene" << std::endl;
}

void MainScene::on_input()
{
	if(IInput::get_key(KeyType::ENTER)->get_keydown())
		SceneManager::getInstance()->switchScene(SceneType::EndMenu);

}
