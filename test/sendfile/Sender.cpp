#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include <PacketAck.h>

#include "Sender.h"

Sender::Sender (size_t _size)
    : Buffer (_size) {
    data_stamps =
        new std::chrono::time_point<std::chrono::steady_clock>[size] ();
}

Sender::~Sender (void) {
    delete[] data_stamps;
}

void Sender::node_prepare (void) {
    buffer_flush ();
}

void Sender::network_data
    (const Packet& _packet, struct sockaddr* _from, socklen_t _len) {

    if (_packet.type.id != PACKET_TYPE_ACK) {
        // Do logging.
        return;
    }

    const PacketAck& packet = _packet.ack;

    if (packet.verify ()) {
        size_t i_win;

        if (accept (packet.seq_num, i_win)) {
            size_t n = i_win + 1;

            win_send_begin -= n;
            shift (n);
        }
    }
}

void Sender::network_timeout (void) {
    // TODO: Send by timeout.
}

void Sender::buffer_flush (void) {
    size_t dirty_begin = (win_begin + data_size) % size;

    if (win_begin > dirty_begin) {
        data_size += read (fd_local, &data[dirty_begin], win_begin - dirty_begin);
    } else if (data_size != size) {
        data_size += read (fd_local, &data[dirty_begin], size - dirty_begin);
        data_size += read (fd_local, &data[0], win_begin);
    }

    // Send data.

    while (win_send_begin < win_size) {
        send_packet (win_send_begin++);
    }
}

void Sender::send_packet (size_t _i_win) const {
    PacketData packet;

    packet.seq_num = cur_seq_num + _i_win;
    
    size_t i_buf = (win_begin + _i_win) % size;
    packet.stx = 0x2;
    packet.data = data[i_buf];
    packet.etx = 0x3;

    packet.prepare ();

    write (fd_net, &packet, sizeof (packet));
}
