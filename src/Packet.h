#pragma once

#include <stdint.h>

#define PACKET_TYPE_ACK  (0x6)
#define PACKET_TYPE_DATA (0x1)

union Packet {
    PacketData data;
    PacketAck ack;
};
