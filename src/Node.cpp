#include <cstdio>
#include <sys/socket.h>

#include "Node.h"

int Node::run (void) {
    
    struct sockaddr from;
    socklen_t from_len;

    Packet packet;

    // Prepare current node.
    node_init ();

    while (alive) {
        int ret_poll = poll (pollfds, pollfds_size, timeout);

        if (ret_poll == -1) {
            // Do logging.
            return -1;
        }
        
        // Receive packet.

        int ret_recv =
            recvfrom (fd_net, &packet, sizeof (packet), 0, &from, &from_len);
        
        if (ret_recv == -1) {
            // Do logging.
            continue;
        }
        
        fprintf (stderr, "Node: got packet!\n");

        switch (packet.type.id) {
        case PACKET_TYPE_DATA:
            network_data (packet.data, &from, from_len);
            break;

        case PACKET_TYPE_ACK:
            network_ack (packet.ack, &from, from_len);
            break;

        default:
            // Do logging.
            break;
        }
    }

    return 0;
}
