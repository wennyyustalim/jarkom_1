#pragma once

#include <stdint.h>

#include "PacketData.h"
#include "PacketAck.h"

#define PACKET_TYPE_ACK  (0x6)
#define PACKET_TYPE_DATA (0x1)

union Packet {
    struct {
        uint8_t id;
    } type;

    PacketData data;
    PacketAck ack;
};
