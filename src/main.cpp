#include "game.hpp"
#include "server/game_manager.hpp"

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        GameManager game_manager;
        game_manager.run();
    }
    else
    {
        Game game;
        game.run();
    }

    return 0;
}
