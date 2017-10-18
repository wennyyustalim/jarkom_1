#pragma once

#include <cstdint>
#include <numeric>

struct PacketData {
    uint8_t soh;
    uint32_t seq_num;
    uint8_t stx;
    uint8_t data;
    uint8_t etx;
    uint8_t checksum;

    bool verify (void) const;
    void prepare (void);
} __attribute__((__packed__));
