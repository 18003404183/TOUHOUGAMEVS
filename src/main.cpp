#include "Core/Game.h"

int main(int argc, char* argv[])
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Game game;
    if (!game.init_game())
    {
        return 0;
    }

    game.start();
    game.end();

    return 0;
}

