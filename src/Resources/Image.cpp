#include "Image.h"

void Image::render(SDLRender& renderer) const
{
    if(!this->data){
        std::cout<<"no data"<<std::endl;
    };
    renderer.draw_texture(this->data,this->pos,this->scale,this->wh,this->rotation,this->alpha);
}

void Image::update(float deltatime){


}