#pragma once

#include <Buffer.h>
#include <PacketData.h>

class Receiver : public Buffer {
public:
    Receiver (int _fd_net, int _fd_loc)
        : fd_net (_fd_net), fd_loc (_fd_loc) {}

    void accept_data (const PacketData& _packet);

private:
    int fd_net;
    int fd_loc;
};
