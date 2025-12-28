#pragma once

#include"Texture.h"
class Atlas
{
public:
    Atlas() = default;
    Atlas(Texture* texture);

private:
    glm::vec2 cell;
    Texture* texture;
};

