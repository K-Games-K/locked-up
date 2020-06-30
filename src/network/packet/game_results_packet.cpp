#include "game_results_packet.hpp"

GameResultsPacket::GameResultsPacket()
    : Packet(PACKET_ID)
{}

GameResultsPacket::GameResultsPacket(std::string murderer, std::string voting_result)
    : Packet(PACKET_ID), murderer(murderer), voting_result(voting_result)
{}

std::string GameResultsPacket::get_murderer() const
{
    return murderer;
}

std::string GameResultsPacket::get_voting_result() const
{
    return voting_result;
}

void GameResultsPacket::serialize(sf::Packet& data) const
{
    data << murderer << voting_result;
}

void GameResultsPacket::deserialize(sf::Packet& data)
{
    data >> murderer >> voting_result;
}
