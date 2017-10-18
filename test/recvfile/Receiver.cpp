#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include <Packet.h>

#include "Receiver.h"

Receiver::Receiver (size_t _size)
    : Buffer (_size) {
    data_flags = new bool[size] ();

    data_size = size;
}

Receiver::~Receiver (void) {
    delete[] data_flags;
}

void Receiver::node_prepare (void) {
    // Do nothing.
}

void Receiver::network_data
    (const Packet& _packet, struct sockaddr* _from, socklen_t _len) {

    if (_packet.type.id != PACKET_TYPE_DATA) {
        // Do logging.
        return;
    }

    const PacketData& packet = _packet.data;

    if (packet.verify ()) {
        size_t i_win;

        if (accept (packet.seq_num, i_win)) {
            size_t i_buf = (win_begin + i_win) % size;

            data[i_buf] = packet.data;
            data_flags[i_buf] = true;

            // Shift if needed.
            while (data_flags[win_begin]) {
                data_flags[win_begin] = false;

                shift (1, false);
            }

            // Manually flush buffer.
            buffer_flush ();
        }
    }

    // Send ACK.

    PacketAck ack;

    ack.seq_num = cur_seq_num - 1;
    ack.win_size = 0;
    ack.prepare ();

    sendto (fd_net, &ack, sizeof (ack), 0, _from, _len);
}

void Receiver::network_timeout (void) {
    // Do nothing.
}

void Receiver::buffer_flush (void) {
    size_t dirty_begin = (win_begin + data_size) % size;

    if (win_begin > dirty_begin) {
        data_size += write (fd_local, &data[dirty_begin], win_begin - dirty_begin);
    } else if (data_size != size) {
        data_size += write (fd_local, &data[dirty_begin], size - dirty_begin);
        data_size += write (fd_local, &data[0], win_begin);
    }
}
