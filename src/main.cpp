#include "Game.h"

int main()
{
    bool init_success = Game::getInstance().Init("LearnOpenGL", 800, 600);
    if (!init_success)
    {
        return -1;
    }

    Game::getInstance().Run();

    return 0;
}