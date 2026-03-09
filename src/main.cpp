#include"Core/Game.h"
int main(int argc,char* argv[]) {

	Game game;
	if(!game.init_game()){
		return 0;
	}
	
	//game.load_resources();
	game.start();
	game.end();
    
	return 0;
}

