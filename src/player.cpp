#include <algorithm>
#include <random>
#include <iostream>
#include <ctime>

#include "player.hpp"

Player::Player(const std::string& nickname, const std::string& avatar_name)
    : nickname(nickname), avatar_name(avatar_name) {}

void Player::set_nickname(const std::string& nickname)
{
    this->nickname = nickname;
}

void Player::set_avatar_name(const std::string& avatar_name)
{
    this->avatar_name = avatar_name;
}

std::string Player::get_nickname() const
{
    return nickname;
}

std::string Player::get_avatar_name() const
{
    return avatar_name;
}

void Player::set_position(int x, int y)
{
    this->position = {x, y};
}

void Player::set_position(sf::Vector2i position)
{
    this->position = position;
}

sf::Vector2i Player::get_position() const
{
    return position;
}

void Player::move(int x, int y)
{
    this->position += {x, y};
}

const std::vector<int>& Player::get_alibi() const
{
    return alibi;
}

void Player::generate_alibi(const GameBoard& game_board, int starting_room, int alibi_length)
{
    size_t rooms_count = game_board.rooms_count();
    std::vector<int> visit_count(rooms_count);

    alibi.clear();
    alibi.push_back(starting_room);

    // Initializing RNG
    static std::mt19937 gen(time(nullptr));
    std::uniform_real_distribution<> rand_percent(0, 100);
    std::uniform_int_distribution<> rand_room(1, rooms_count - 1);

    int current_room = starting_room;
    for(int i = 0; i < alibi_length - 1; ++i)
    {
        std::uniform_int_distribution<> rand_neighour(
            0, game_board.get_neighbours(current_room).size() - 1
        );
        int stay_prob = rand_percent(gen);
        if(stay_prob < 40.0
            || (alibi.size() >= 2 && alibi[alibi.size() - 2] == alibi[alibi.size() - 1]))
        {
            int neighbour;
            do
            {
                neighbour = game_board.get_neighbours(current_room)[rand_neighour(gen)];
            }
            while(game_board.get_neighbours(current_room).size() > 1 && visit_count[neighbour] > 2);

            current_room = neighbour;
        }

        visit_count[current_room]++;
        alibi.push_back(current_room);
    }

    std::reverse(alibi.begin(), alibi.end());
}

bool Player::operator==(const Player& other) const
{
    return nickname == other.nickname;
}

bool Player::operator<(const Player& other) const
{
    return nickname < other.nickname;
}

std::set<int> Player::get_pmove_pos() const
{
    return pmove_pos;
}
void Player::set_pmove_pos(std::set<int> positions)
{
    pmove_pos = positions;
}