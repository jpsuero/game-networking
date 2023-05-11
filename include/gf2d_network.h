#ifndef __gf2d_NETWORK_H__
#define __gf2d_NETWORK_H__

/**
 * @brief initializes enet
*/
int gf2d_network_init();


/**
 * @brief creates a host server
 * @param port port to be used for host
 * @param max_connections max number of coonnections
 * @return 1 on success, 0 on failure
*/
int gf2d_network_host(int port, int max_connections);

int gf2d_network_client();

/**
 * @brief will broadcast a packet to all peers connected to host
 * @param host on which to broadcast the packet
 * @param channelID channel on which to broadcast
 * @param data packet to broadcast 
 * 
*/
void gf2d_network_broadcast_packet();


/**
 * @brief send a packet to a specific peer
 * @param peer to send packet to
 * @param data packet to send
 * @param size size of packet 
*/
void gf2d_network_send_packet();

void gf2d_network_disconnect();

#endif