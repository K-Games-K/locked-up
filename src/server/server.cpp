#include "server.hpp"
#include "../logging.hpp"
#include "../network/packet/packets.hpp"

Server::Server(sf::IpAddress bind_addr, unsigned short bind_port,
    PacketReceivedCallback packet_received_callback)
    : packet_received_callback(packet_received_callback)
{
    Log::info() << "Initializing game server..." << std::endl;

    sf::Socket::Status status = listener.listen(bind_port, bind_addr);
    listener.setBlocking(false);

    std::string bind_str = bind_addr.toString() + ":" + std::to_string(bind_port);
    if(status != sf::Socket::Done)
    {
        Log::error() << "Failed to bind to " << bind_str << "!" << std::endl;
        return;
    }

    Log::info() << "Listening on " << bind_str << "..." << std::endl;

    enabled = true;
    Log::info() << "Game server initialized successfully!" << std::endl;
}

bool Server::is_enabled() const
{
    return enabled;
}

void Server::update()
{
    if(!enabled)
        return;

    bool players_list_changed = false;

    if(allow_new_connections)
    {
        // Looking for new incoming connections.
        auto incoming = std::make_unique<sf::TcpSocket>();
        while(listener.accept(*incoming) == sf::Socket::Done)
        {
            Log::info() << "Incoming connection from " << incoming->getRemoteAddress() << std::endl;

            pending_connections.emplace_back(std::move(incoming));
        }

        // Looking for JoinGamePackets from pending connections.
        for(auto it = pending_connections.begin(); it != pending_connections.end(); ++it)
        {
            auto& connection = *it;

            auto packet = connection.recv();
            if(!connection.is_connected())
            {
                Log::info() << connection.get_addr() << " lost connection!" << std::endl;

                it = --pending_connections.erase(it);
                continue;
            }

            if(packet == nullptr)
                continue;

            if(packet->get_id() == JoinGamePacket::PACKET_ID)
            {
                auto join_game_packet = dynamic_cast<JoinGamePacket&>(*packet);

                Log::info() << "Player " << join_game_packet.get_nickname() << " ("
                    << connection.get_addr() << ") joined the server!" << std::endl;

                connected_players.emplace_back(
                    connected_players.size(),
                    join_game_packet.get_nickname(),
                    join_game_packet.get_avatar_name(),
                    std::move(connection)
                );
                it = --pending_connections.erase(it);
                players_list_changed = true;

                if(packet_received_callback)
                    packet_received_callback(
                        connected_players[connected_players.size() - 1], std::move(packet)
                    );
            }
        }
    }

    for(auto it = connected_players.begin(); it != connected_players.end(); ++it)
    {
        auto& player = *it;
        auto& connection = player.get_connection();

        auto packet = connection.recv();
        if(!connection.is_connected())
        {
            Log::info() << "Player " << player.get_nickname() << " lost connection!" << std::endl;

            it = --connected_players.erase(it);
            players_list_changed = true;
            continue;
        }

        if(packet == nullptr)
            continue;

        switch(packet->get_id())
        {
            case DebugPacket::PACKET_ID:
            {
                auto debug_packet = dynamic_cast<DebugPacket&>(*packet);
                Log::debug() << player.get_nickname() << " (" << connection.get_addr() << "): "
                    << debug_packet.get_message() << std::endl;

                continue;
            }
            case DisconnectPacket::PACKET_ID:
            {
                auto disconnect_packet = dynamic_cast<DisconnectPacket&>(*packet);

                Log::info() << "Player " << player.get_nickname() << " left the server. Reason: "
                    << disconnect_packet.get_reason() << std::endl;

                it = --connected_players.erase(it);
                players_list_changed = true;

                continue;
            }
            default:
                break;
        }

        if(packet_received_callback)
            packet_received_callback(player, std::move(packet));
    }

    if(players_list_changed)
        update_players_list();
}

void Server::broadcast(const Packet& packet)
{
    for(auto& player : connected_players)
        player.get_connection().send(packet);
}

void Server::set_allow_new_connections(bool allow_new_connections)
{
    this->allow_new_connections = allow_new_connections;
    pending_connections.clear();
}

void Server::update_players_list()
{
    std::vector<Player> players_list(connected_players.begin(), connected_players.end());
    for(int i = 0; i < connected_players.size(); ++i)
    {
        connected_players[i].set_player_id(i);
        connected_players[i].get_connection().send(PlayersListPacket(i, players_list));
    }
}

std::vector<RemotePlayer>& Server::get_connected_players()
{
    return connected_players;
}

void Server::reset()
{
    for(auto& player : connected_players)
        pending_connections.push_back(std::move(player.get_connection()));

    connected_players.clear();
    allow_new_connections = true;
}
