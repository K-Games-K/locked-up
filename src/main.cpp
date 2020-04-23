#include "game.hpp"
#include "network/server.hpp"

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        Server server(2704);
        for(;;)
            server.update();
    }
    else
    {
        Game game;
        game.run();
    }

    return 0;
}
