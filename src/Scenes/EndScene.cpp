#include "EndScene.h"


EndScene::EndScene() {
	// Constructor implementation
	std::cout << "EndScene created" << std::endl;
}

EndScene::~EndScene() {
	// Destructor implementation
	std::cout << "EndScene destroyed" << std::endl;
}

void EndScene::on_enter() {
	
	std::cout << "EndScene::on_enter() called" << std::endl;
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
}

void EndScene::on_exit() {
	std::cout << "EndScene::on_exit() called" << std::endl;
}

void EndScene::on_update(float delta_time) {
	std::cout << "EndScene::on_update() called with delta_time: " << delta_time << std::endl;
}

void EndScene::on_render(SDLRender& renderer) {
	std::cout << "EndScene::on_render() called" << std::endl;
}

void EndScene::on_input() {
	std::cout << "EndScene::on_input() called" << std::endl;
}

