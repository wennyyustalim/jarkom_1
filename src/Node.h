#pragma once

#include <poll.h>

#include <Packet.h>

class Node {
public:
    int run (void);

    static const size_t pollfds_size = 1;

    int& fd_net = pollfds[0].fd;
    int fd_local;

protected:
    bool alive = true;
    int timeout = -1;

    struct pollfd pollfds[pollfds_size] = {
        {-1, POLLIN, 0}
    };

    virtual void node_init (void) = 0;

    virtual void network_data
        (const Packet& _packet, struct sockaddr* _from, socklen_t _len) = 0;
    virtual void network_timeout (void) = 0;
};
