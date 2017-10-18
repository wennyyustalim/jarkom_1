#include <algorithm>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include <PacketAck.h>

#include "Sender.h"

Sender::Sender (size_t _size)
    : Buffer (_size) {
    data_stamps = new Timestamp[size] ();
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
        fprintf (stderr, "Sender: Receive ACK seq_num=%u\n", packet.seq_num);

        size_t i_win;

        if (accept (packet.seq_num, i_win)) {
            fprintf (stderr, "Sender: Accept ACK i_win=%lu\n", i_win);

            size_t n = i_win + 1;

            win_send_begin -= n;
            shift (n);
        }
    }
}

void Sender::network_timeout (void) {
    Timestamp curr = std::chrono::steady_clock::now ();
    Timestamp next = curr + packet_timeout;

    for (size_t i_win = 0; i_win < win_send_begin; i_win++) {
        size_t i_buf = (win_begin + i_win) % size;
        Timestamp tm = data_stamps[i_buf];

        if (curr > tm) {
            fprintf (stderr, "Sender: Timeout i_win=%lu\n", i_win);
            send_packet (i_win, next);
        } else {
            int mil =
                std::chrono::duration_cast<std::chrono::milliseconds> (curr - tm).count ();
            timeout = (timeout > 0) ? std::min (timeout, mil) : mil;
        }
    }
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

    Timestamp next = std::chrono::steady_clock::now () + packet_timeout;

    while (win_send_begin < win_size) {
        send_packet (win_send_begin++, next);
    }
}

void Sender::send_packet (size_t _i_win, Timestamp _stamp) {
    PacketData packet;

    size_t i_buf = (win_begin + _i_win) % size;

    data_stamps[i_buf] = _stamp;

    packet.seq_num = cur_seq_num + _i_win;
    packet.stx = 0x2;
    packet.data = data[i_buf];
    packet.etx = 0x3;

    packet.prepare ();

    int mil = std::chrono::milliseconds (packet_timeout).count ();
    timeout = (timeout > 0) ? std::min (timeout, mil) : mil;

    fprintf (stderr, "Sender: Send DATA seq_num=%u\n", packet.seq_num);

    write (fd_net, &packet, sizeof (packet));
}
