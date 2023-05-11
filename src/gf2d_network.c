#include <enet/enet.h>
#include <stdio.h>
#include "simple_logger.h"

int hosting = 0;
int connected = 0;


ENetHost* host;
ENetPeer* peer;



int gf2d_network_init() 
{
  slog("initializing network");
  if(enet_initialize() != 0)
  {
    slog("an error occurred while initializing ENet");
    return;
  }
  atexit(enet_deinitialize);
  slog("initialized network");
}

int gf2d_network_host(int port, int max_connections)
{
    ENetAddress address;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = port;

     host = enet_host_create (&address	/* the address to bind the server host to */,
    				max_connections	/* allow up to max_connections clients and/or outgoing connections */,
    				1	/* allow up to 1 channel to be used. */,
    				0	/* assume any amount of incoming bandwidth */,
    				0	/* assume any amount of outgoing bandwidth */);

    if(host == NULL)
    {
      slog("an error occurred while trying to create an ENet server host");
      return;
    }

}

void gf2d_network_broadcast_packet(const char* data, size_t size)
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
        //store needed client information

        //allocate storage
        event.peer->data = (char*)malloc(255 *sizeof(char));
        break;

      case ENET_EVENT_TYPE_RECEIVE:
        slog("A packet of length %u containing %s was received from %s on channel %u\n", 
        event.packet->dataLength,
        event.packet->data,
        event.peer->data,
        event.channelID);

        enet_packet_destroy(event.packet);
        break;
      
      case ENET_EVENT_TYPE_DISCONNECT:
        slog("%s disconnected.\n", event.peer->data);
        free(event.peer->data);
        event.peer->data = NULL;
        break;
    }

  }
}

void gf2d_network_connect(const char* address_str, unsigned int port)
{
  ENetAddress  address;
  ENetEvent event;

  host = enet_host_create(NULL, 1, 1, 0, 0);

}







