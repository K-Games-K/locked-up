#pragma once

#include <SFML/Network.hpp>

#include "game_board.hpp"
#include "remote_player.hpp"
#include "network/packets.hpp"

class Server
{
public:
    using PacketReceivedCallback = std::function<void(RemotePlayer&, const Packet::Any&)>;

    Server(sf::IpAddress bind_addr, unsigned short bind_port,
        PacketReceivedCallback packet_received_callback);

    bool is_enabled() const;

    void update();

    void broadcast(const Packet::Packet& packet);

    void update_players_list();

    void set_allow_new_connections(bool allow_new_connections);

    std::vector<RemotePlayer>& get_connected_players();

    void reset();

private:
    bool enabled = false;
    bool allow_new_connections = true;

    PacketReceivedCallback packet_received_callback;

    sf::TcpListener listener;

    std::vector<Connection> pending_connections;
    std::vector<RemotePlayer> connected_players;
};
