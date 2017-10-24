#pragma once

#include <cstdint>

struct PacketAck {
    uint8_t ack;
    uint32_t seq_num;
    uint8_t win_size;
    uint8_t checksum;

    bool verify (void) const;
    void prepare (void);
} __attribute__((__packed__));
