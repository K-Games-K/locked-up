#include <iostream>

#include "network/server.hpp"

Server::Server(unsigned short bind_port, sf::IpAddress bind_addr)
{
    listener.listen(bind_port, bind_addr);
    listener.setBlocking(false);
}

void Server::update()
{
    auto incoming = std::make_shared<sf::TcpSocket>();
    if(listener.accept(*incoming) == sf::Socket::Done)
    {
        new_connection(Connection(incoming));
    }

    for(auto& connection : connections)
    {
        auto packet = connection.recv();
        if(packet == nullptr)
            continue;

        packet_received(connection, std::move(packet));
    }
}

void Server::new_connection(Connection connection)
{
    std::cout << "New connection from: " << connection.get_addr() << std::endl;

    connections.push_back(connection);
}

void Server::packet_received(Connection& sender, std::unique_ptr<Packet> packet)
{
    std::cout << "[" << sender.get_addr() << "] sent a packet!" << std::endl;
}


