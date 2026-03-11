#include "MainScene.h"
#include "SceneManager.h"
// #include "EasyxRender.h"
#include "Animation.h"
#include "GameContext.h"
#include "ResourcesManager.h"
#include "clock.h"
#include "Danmaku.h"

MainScene::MainScene() {
  IInput::get();
  std::cout << "MainScene Constructor" << std::endl;
}

MainScene::~MainScene() { std::cout << "MainScene Destructor" << std::endl; }

void MainScene::on_enter() {
  ResourcesManager::getInstance()->loadScene(SceneType::MainMenu);
  ColliderManager::get_instance();
  
  auto t = std::make_unique<DanmakuPool>(10000);
  ResourcesManager::getInstance()->register_danmaku_texture("");
  DanmakuPrefab dp;
  dp.base_radius = 0;
  dp.base_scale = {1,1};
  dp.shape_type = DanmakuShape::Circle;


  std::cout << "Entering MainScene" << std::endl;
  EnemyBuilder enemybuilder;
  enemybuilder.create_enemy(EnemyType::small);
  Enemy *a = enemybuilder.create();
  this->renderables.push_back(a);
  this->updateables.push_back(a);

  enemybuilder.create_enemy(EnemyType::common);
  Enemy *b = enemybuilder.create();
  this->renderables.push_back(b);
  this->updateables.push_back(b);

  enemybuilder.create_enemy(EnemyType::large);
  Enemy *c = enemybuilder.create();
  this->renderables.push_back(c);
  this->updateables.push_back(c);

  // 调用this后relese模式下程序暂停 debug没有问题

  this->clock.set_callback([this]() {
    std::cout << this << std::endl;                           // 没问题
    std::cout << "daugsjdha" << "this:" << this << std::endl; // 没问题
    std::cout << "踹刷u读书u和古代u上高低" << "this:" << this
              << std::endl; // 没问题
    // std::cout<<"踹刷u读书u和古代u上高低"<<this<<std::endl;//程序强制暂停
  });

  Texture *v = ResourcesManager::getInstance()->get_texture("resources\\2.png");
  glm::vec2 pos;
  glm::vec2 scale;
  pos.x = 205;
  pos.y = 205;
  scale.x = 1;
  scale.y = 1;

  Image player_image = Image(v, pos, scale, 0, 255);
  Atlas *player_atlas =
      ResourcesManager::getInstance()->get_atlas("resources\\6.png");

  Context::Instances()->set_player_context(pos, {0, 0}, true, true, 100);
  PlayerContext pc = Context::Instances()->get_player_context();
  this->player = new Player(pos,{0,0},100);

  player->setImage(player_image);
  // player->set_atlas(player_atlas);
  Animation animation(player_atlas, 0.1);

  Collider* player_collider = ColliderManager::get_instance()->create_collider(new Circle(10),0,player);

  this->renderables.push_back(player);
  this->updateables.push_back(player);
  this->player->set_animation(animation);
  player->set_collider(player_collider);
  player->get_collider()->set_on_collid([p = this->player](Collider* other){
      p->set_alive(false);
			std::cout << "die" << std::endl; 
		});

}

void MainScene::on_exit() { 
  std::cout << "Exiting MainScene" << std::endl;
  Context::Instances()->set_player_context(
      player->get_position(), player->get_velocity(), player->isActive(),
      player->is_alive(), player->get_hp());
}

void MainScene::on_update(float deltatime) {
  for (IUpdateable *a : this->updateables) {
    a->update(deltatime);
  }
  this->clock.update(deltatime);
  IInput::update();

  ColliderManager::get_instance()->check_and_resolve_collisions();
}

void MainScene::on_render(SDLRender &renderer) {



  for (IRenderable *a : this->renderables) {
    a->render(renderer);
  }
  
}

void MainScene::on_input() {
  if (IInput::get_key(KeyType::ENTER)->get_keydown())
    SceneManager::getInstance()->switchScene(SceneType::EndMenu);
  if (IInput::get_key(KeyType::RIGHT)->get_keydown())
    this->player->set_velocity({10, 0});
  if (IInput::get_key(KeyType::LEFT)->get_keydown())
    this->player->set_velocity({-10,0});
  if (IInput::get_key(KeyType::DOWN)->get_keydown())
    this->player->set_velocity({0,10});
  if (IInput::get_key(KeyType::UP)->get_keydown())
    this->player->set_velocity({0,-10});
}
