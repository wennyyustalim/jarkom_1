#pragma once

#include <Node.h>
#include <Buffer.h>

class Receiver : public Node, public Buffer {
public:
    Receiver (size_t _size = 256)
        : Buffer (_size) {}

protected:
    void node_init (void);

    void network_data
        (const PacketData& _packet, struct sockaddr* _from, socklen_t _len);
    void network_ack
        (const PacketAck& _packet, struct sockaddr* _from, socklen_t _len);
};
