#pragma once

#include"Texture.h"
#include"includes.h"

//分割texture 成为atlas

class Atlas
{
public:
    Atlas() = default;
    Atlas(Texture* texture);

private:
    //glm::vec2 cell;
    Texture* texture;
};

