#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>

#include "Node.h"

int Node::run (void) {

    struct sockaddr from;
    socklen_t len;

    Packet packet;

    // Prepare current node.
    node_prepare ();

    while (alive) {
        int ret_poll = poll (pollfds, pollfds_size, timeout);

        // Reset timeout.
        timeout = -1;

        if (ret_poll == -1) {
            // Do logging.
            return -1;
        }

        // Receive packet.

        if (ret_poll > 0) {
            int ret_recv =
                recvfrom (fd_net, &packet, sizeof (packet), 0, &from, &len);

            if (ret_recv == -1) {
                fprintf
                    (stderr, "Node: Unable to receive packet: %s\n", strerror (errno));

                // Do logging.
                continue;
            }

            network_data (packet, &from, len);
        } else {
            network_timeout ();
        }
    }

    return 0;
}
