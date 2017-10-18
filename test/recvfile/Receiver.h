#pragma once

#include <Node.h>
#include <Buffer.h>

class Receiver : public Node, public Buffer {
public:
    Receiver (size_t _size = 256);

    Receiver (const Receiver& _src) = delete;
    Receiver& operator= (const Receiver& _src) = delete;

    virtual ~Receiver (void);

protected:
    void node_init (void);

    void network_data
        (const Packet& _packet, struct sockaddr* _from, socklen_t _len);
    void network_timeout (void);

    void buffer_flush (void);

    bool* data_flags;
};
