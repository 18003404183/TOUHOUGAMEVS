#include"SDLWindow.h"

bool SDLWindows::init(int w,int h){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
		std::cout<<"cant init:"<<std::endl;
		return false;
	}

	this->window = SDL_CreateWindow(
		"TOUHOUGAME",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		SDL_WINDOW_SHOWN
	);

	if(window == nullptr){
		std::cerr<<"Window could not be created! SDL_Error"<<SDL_GetError();
		return false;
	}
    return true;

}

SDL_Window* SDLWindows::get_sdl_window(){
    return this->window;
}
