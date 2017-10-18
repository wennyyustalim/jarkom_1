#pragma once

#include <Node.h>
#include <Buffer.h>

class Receiver : public Node, public Buffer<char> {
public:
    Receiver (int _fd_net, int _fd_local)
        : Node (_fd_net, _fd_local), Buffer () {}

protected:
    void node_init (void);

    void network_data
        (const PacketData& _packet, struct sockaddr* _from, socklen_t _len);
    void network_ack
        (const PacketAck& _packet, struct sockaddr* _from, socklen_t _len);
};
