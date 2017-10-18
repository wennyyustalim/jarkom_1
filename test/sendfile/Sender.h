#pragma once

#include <chrono>

#include <Node.h>
#include <Buffer.h>

class Sender : public Node, public Buffer {
public:
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

    std::chrono::time_point<std::chrono::steady_clock>* data_stamps;
    
    size_t win_send_begin = 0;

private:
    void send_packet (size_t _i_win) const;
};
