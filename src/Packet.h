#pragma once

#include <stdint.h>

#define PACKET_TYPE_ACK  (0x6)
#define PACKET_TYPE_DATA (0x1)

union Packet {
    struct {
        uint8_t id;
    } type __attribute__((__packed__));

    PacketData data;
    PacketAck ack;
};
