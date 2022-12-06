#include <enet/enet.h>
#include <stdio.h>
#include "simple_logger.h"

int gf2d_network_init() 
{
  slog("initializing network");
  if(enet_initialize() != 0)
  {
    slog("an error occurred while initializing ENet");
    return EXIT_FAILURE;
  }
  exit(enet_deinitialize);
  slog("initialized network");
}

int gf2d_network_host(int port, int max_connections)
{
    ENetAddress address;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = port;

     ENetHost * server = enet_host_create (&address	/* the address to bind the server host to */,
    				max_connections	/* allow up to max_connections clients and/or outgoing connections */,
    				1	/* allow up to 1 channel to be used. */,
    				0	/* assume any amount of incoming bandwidth */,
    				0	/* assume any amount of outgoing bandwidth */);

    if(server == NULL)
    {
      slog("an error occurred while trying to create an ENet server host");
      return EXIT_FAILURE;
    }

}

void gf2d_network_broadcast_packet(ENetHost * host, const char* data, size_t size)
{
  ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);

  enet_host_broadcast(host, 0, packet);
}

void gf2d_network_send_packet(ENetPeer * peer, const char* data, size_t size)
{
  ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, 0, packet);
}

void gf2d_network_poll(ENetHost *host)
{
  ENetEvent event;
  while(enet_host_service(host, &event, 0) > 0)
  {
    switch(event.type)
    {
      case ENET_EVENT_TYPE_CONNECT:
        slog("A new client connected from %x", event.peer->address.host);
    }
  }
}







