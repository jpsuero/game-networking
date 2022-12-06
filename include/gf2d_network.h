#ifndef __GF2D_NETWORK_H__
#define __GF2D_NETWORK_H__

/**
 * @brief initializes enet
*/
int gf2d_network_init();


/**
 * @brief create a server host
 * @param port port to be used for host
 * @param max_connections max number of coonnections
 * @return 1 on success, 0 on failure
*/
int gf2d_network_host(int port, int max_connections);

int gf2d_network_client();

void gf2d_network_broadcast_packet();

void gf2d_network_send_packet();

void gf2d_network_disconnect();

#endif