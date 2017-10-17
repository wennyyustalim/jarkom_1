#include "Receiver.h"

void Receiver::accept_data (const PacketData& _packet) {

    if (_packet.verify ()) {
        char* data = slide (_packet.seq_num);

        if (data != nullptr) {
            (*data) = _packet.data;
        }
    }

    // Send ACK.

    PacketAck ack;

    ack.seq_num = cur_seq_num;
    ack.win_size = 0;
    ack.prepare ();

    write (fd_net, &ack, sizeof (PacketAck));

    // Handle dirty parts.

    size_t dirty_begin = (begin + data_size) % size;

    if (begin > dirty_begin) {
        data_size += write (fd_loc, &data[dirty_begin], begin - dirty_begin);
    } else if (data_size != size) {
        data_size += write (fd_loc, &data[dirty_begin], size - dirty_begin);
        data_size += write (fd_loc, &data[0], begin);
    }
}
