#include <SFML/Window.hpp>

#include "game.hpp"
#include "state/play_state.hpp"
#include "state/main_menu_state.hpp"

Game::Game()
        : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                 WINDOW_NAME,
                 sf::Style::Close | sf::Style::Fullscreen,
                 sf::ContextSettings(0, 0, 16))
{}

void Game::run()
{
    game_state_manager.push_state(new MainMenuState(window, game_state_manager));
    sf::Clock clock;
    while(window.isOpen())
    {
        const float dt = clock.restart().asSeconds();

        game_state_manager.update();
        GameState* current_state = game_state_manager.get_current_state();
        if(current_state == nullptr)
            break;

        // Handling events.
        sf::Event event;
        if(window.pollEvent(event))
            current_state->handle_input(event);

        // Updating game logic.
        current_state->update(dt);

        // Rendering.
        current_state->render(dt);
        window.display();

        sf::sleep(sf::milliseconds(5));
    }
}
