#include <SFML/Window.hpp>

#include "game.hpp"
#include "state/play_state.hpp"

Game::Game()
        : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                 WINDOW_NAME,
                 sf::Style::Close),
          current_state(std::make_unique<PlayState>(window))
{}

void Game::run()
{
    sf::Clock clock;
    while(window.isOpen())
    {
        const float dt = clock.restart().asSeconds();

        // Handling events.
        sf::Event event;
        if(window.pollEvent(event))
        {
            auto new_state = current_state->handle_input(event);
            if(new_state != nullptr)
            {
                current_state = std::move(new_state);
                continue;
            }
        }

        // Updating game logic.
        auto new_state = current_state->update(dt);
        if(new_state != nullptr)
        {
            current_state = std::move(new_state);
            continue;
        }

        // Rendering.
        current_state->render(dt);

        sf::sleep(sf::milliseconds(10));
    }
}
