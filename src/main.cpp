#define _USE_MATH_DEFINES
#include "game.hpp"
#include "server/server.hpp"

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        Server server(2704);
        if(!server.is_enabled())
            return -1;
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
