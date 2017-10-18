#pragma once

#include <chrono>

#include "Node.h"
#include "Buffer.h"

class Sender : public Node, public Buffer {
public:
    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

    const std::chrono::milliseconds packet_timeout =
        std::chrono::milliseconds (100);

    Sender (size_t _size = 256);

    Sender (const Sender& _src) = delete;
    Sender& operator= (const Sender& _src) = delete;

    virtual ~Sender (void);

protected:
    void node_prepare (void);
    
    void network_data
        (const Packet& _packet, struct sockaddr* _from, socklen_t _len);
    void network_timeout (void);

    void buffer_flush (void);

    Timestamp* data_stamps;
    
    size_t win_send_begin = 0;

    size_t data_eof = 0;
    size_t seqn_eof = 0;
    bool flag_eof = false;

private:
    void send_packet (size_t _i_win, Timestamp _stamp);
};
