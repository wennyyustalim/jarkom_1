#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include <Packet.h>

#include "Receiver.h"

void Receiver::node_init (void) {
    data_size = size;
}

void Receiver::network_data
    (const Packet& _packet, struct sockaddr* _from, socklen_t _len) {

    if (_packet.type.id != PACKET_TYPE_DATA) {
        // Do logging.
        return;
    }

    const PacketData& packet = _packet.data;

    if (packet.verify ()) {
        char* data = slide (packet.seq_num);

        fprintf (stderr, "Receiver: data=%p\n", data);

        if (data != nullptr) {
            (*data) = packet.data;
        }
    }

    // Send ACK.

    PacketAck ack;

    ack.seq_num = cur_seq_num;
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
