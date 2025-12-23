#include"Core/Game.h"
int main() {

	Game game;
	game.init_game();
	game.load_resources();
	game.start();
    
	return 0;
}