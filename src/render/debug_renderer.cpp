#include "render/debug_renderer.hpp"

DebugRenderer::DebugRenderer(sf::RenderWindow& window, ResourceManagers resources)
        : Renderer(window, resources)
{}

void DebugRenderer::render(const DebugContext& context, sf::Vector2f camera_pos)
{
    sf::Font& font = resources.fonts.get("IndieFlower-Regular");
    GameBoard& game_board = context.game_board;
    Player& player = context.player;

    sf::Vector2f game_board_viewport = game_board_size / (float) TILE_SIZE;
    sf::Vector2f offset = camera_pos - game_board_position / TILE_SIZE;

    for(int y = (int) camera_pos.y;
            y < game_board.get_height() && y <= camera_pos.y + game_board_viewport.y; ++y)
    {
        for(int x = (int) camera_pos.x;
                x < game_board.get_width() && x <= camera_pos.x + game_board_viewport.x; ++x)
        {
            if(!game_board.can_move(x, y, 1, 0))
            {
                sf::VertexArray arr(sf::LineStrip, 4);
                arr[0].position = sf::Vector2f(x + 1 - offset.x, y - offset.y) * TILE_SIZE;
                arr[0].color = sf::Color::Red;
                arr[1].position = sf::Vector2f(x + 1 - offset.x, y + 1 - offset.y) * TILE_SIZE;
                arr[1].color = sf::Color::Red;
                arr[2].position = arr[1].position + sf::Vector2f(1, 0);
                arr[2].color = sf::Color::Red;
                arr[3].position = arr[0].position + sf::Vector2f(1, 0);
                arr[3].color = sf::Color::Red;

                window.draw(arr);
            }
            if(!game_board.can_move(x, y, 0, 1))
            {
                sf::VertexArray arr(sf::LineStrip, 4);
                arr[0].position = sf::Vector2f(x - offset.x, y + 1 - offset.y) * TILE_SIZE;
                arr[0].color = sf::Color::Red;
                arr[1].position = sf::Vector2f(x + 1 - offset.x, y + 1 - offset.y) * TILE_SIZE;
                arr[1].color = sf::Color::Red;
                arr[2].position = arr[1].position + sf::Vector2f(0, 1);
                arr[2].color = sf::Color::Red;
                arr[3].position = arr[0].position + sf::Vector2f(0, 1);
                arr[3].color = sf::Color::Red;

                window.draw(arr);
            }

            // Rendiring name of room on each tile.
            // std::string str = game_board.get_room(x, y).get_name().substr(0, 6);
            // sf::Text name(str, font, 12);
            // name.setPosition((x - offset.x) * TILE_SIZE + 2, (y - offset.y) * TILE_SIZE + 2);
            // window.draw(name);
        }
    }

    std::string str = "player_id: " + std::to_string(context.player_id) + "\nx: " +
            std::to_string(player.get_position().x) + " y: " +
            std::to_string(player.get_position().y) + "\nroom: " + game_board.get_room(
            player.get_position().x,
            player.get_position().y
    ).get_name();
    sf::Text text(str, font, 25);
    text.setPosition(5, 5);
    window.draw(text);
}

void DebugRenderer::set_game_board_position(sf::Vector2f game_board_position)
{
    this->game_board_position = game_board_position;
}

void DebugRenderer::set_game_board_size(sf::Vector2f game_board_size)
{
    this->game_board_size = game_board_size;
}
