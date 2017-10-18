#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>

#include <PacketAck.h>

#include "Receiver.h"

void Receiver::node_init (void) {
    data_size = size;
}

void Receiver::network_data
    (const PacketData& _packet, struct sockaddr* _from, socklen_t _len) {

    if (_packet.verify ()) {
        char* data = slide (_packet.seq_num);

        fprintf (stderr, "Receiver: data=%p\n", data);

        if (data != nullptr) {
            (*data) = _packet.data;
        }
    }

    // Send ACK.

    PacketAck ack;

    ack.seq_num = cur_seq_num;
    ack.win_size = 0;
    ack.prepare ();

    sendto (fd_net, &ack, sizeof (ack), 0, _from, _len);

    // Handle dirty parts.

    size_t dirty_begin = (win_begin + data_size) % size;

    if (win_begin > dirty_begin) {
        fprintf (stderr, "Receiver: win_begin > dirty_begin\n");
        data_size += write (fd_local, &data[dirty_begin], win_begin - dirty_begin);
    } else if (data_size != size) {
        fprintf (stderr, "Receiver: data_size != size\n");
        data_size += write (fd_local, &data[dirty_begin], size - dirty_begin);
        data_size += write (fd_local, &data[0], win_begin);
    }
}

void Receiver::network_ack
    (const PacketAck& _packet, struct sockaddr* _from, socklen_t _len) {
    // Do nothing.
}
